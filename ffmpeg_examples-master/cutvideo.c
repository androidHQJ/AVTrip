#include <stdlib.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>

static void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt, const char *tag)
{
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

    printf("%s: pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           tag,
           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
           pkt->stream_index);
}

int cut_video(double from_seconds, double end_seconds, const char* in_filename, const char* out_filename) {
    AVOutputFormat *ofmt = NULL;
    AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;
    AVPacket pkt;
    int ret, i;

    av_register_all();

    if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {
        fprintf(stderr, "Could not open input file '%s'", in_filename);
        goto end;
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
        fprintf(stderr, "Failed to retrieve input stream information");
        goto end;
    }

    av_dump_format(ifmt_ctx, 0, in_filename, 0);

    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename);
    if (!ofmt_ctx) {
        fprintf(stderr, "Could not create output context\n");
        ret = AVERROR_UNKNOWN;
        goto end;
    }

    ofmt = ofmt_ctx->oformat;

    /**流的拷贝*/

    //拿到输入文件有多少路流 nb_streams
    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        AVStream *in_stream = ifmt_ctx->streams[i];
        //输入文件的每一路流 都要创建出一个 输出流
        AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);
        if (!out_stream) {
            fprintf(stderr, "Failed allocating output stream\n");
            ret = AVERROR_UNKNOWN;
            goto end;
        }
        //输入流的codec信息拷贝到输出流的codec信息中
        ret = avcodec_copy_context(out_stream->codec, in_stream->codec);
        if (ret < 0) {
            fprintf(stderr, "Failed to copy context from input to output stream codec context\n");
            goto end;
        }
        out_stream->codec->codec_tag = 0;
        if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
            out_stream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    av_dump_format(ofmt_ctx, 0, out_filename, 1);

    if (!(ofmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            fprintf(stderr, "Could not open output file '%s'", out_filename);
            goto end;
        }
    }

    //写入多媒体头
    ret = avformat_write_header(ofmt_ctx, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when opening output file\n");
        goto end;
    }

    //    int indexs[8] = {0};


    //跳到指定截取的时间秒的地方
    //    int64_t start_from = 8*AV_TIME_BASE;
    ret = av_seek_frame(ifmt_ctx, -1, from_seconds*AV_TIME_BASE, AVSEEK_FLAG_ANY);
    if (ret < 0) {
        fprintf(stderr, "Error seek\n");
        goto end;
    }

    int64_t *dts_start_from = malloc(sizeof(int64_t) * ifmt_ctx->nb_streams);
    memset(dts_start_from, 0, sizeof(int64_t) * ifmt_ctx->nb_streams);
    int64_t *pts_start_from = malloc(sizeof(int64_t) * ifmt_ctx->nb_streams);
    memset(pts_start_from, 0, sizeof(int64_t) * ifmt_ctx->nb_streams);

    while (1) {
        AVStream *in_stream, *out_stream;

        //读取包
        ret = av_read_frame(ifmt_ctx, &pkt);
        if (ret < 0)
            break;

        in_stream  = ifmt_ctx->streams[pkt.stream_index];
        out_stream = ofmt_ctx->streams[pkt.stream_index];

        log_packet(ifmt_ctx, &pkt, "in");

        //包的时间戳与截取的末尾时间比较 如果超过则结束循环
        if (av_q2d(in_stream->time_base) * pkt.pts > end_seconds) {
            av_free_packet(&pkt);
            break;
        }

        if (dts_start_from[pkt.stream_index] == 0) {
            dts_start_from[pkt.stream_index] = pkt.dts;
            printf("dts_start_from: %s\n", av_ts2str(dts_start_from[pkt.stream_index]));
        }
        if (pts_start_from[pkt.stream_index] == 0) {
            pts_start_from[pkt.stream_index] = pkt.pts;
            printf("pts_start_from: %s\n", av_ts2str(pts_start_from[pkt.stream_index]));
        }
        
        //时间基做转换，保证同步
        /* copy packet */
        pkt.pts = av_rescale_q_rnd(pkt.pts - pts_start_from[pkt.stream_index], in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX);
        pkt.dts = av_rescale_q_rnd(pkt.dts - dts_start_from[pkt.stream_index], in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX);
        if (pkt.pts < 0) {
            pkt.pts = 0;
        }
        if (pkt.dts < 0) {
            pkt.dts = 0;
        }
        pkt.duration = (int)av_rescale_q((int64_t)pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos = -1;
        log_packet(ofmt_ctx, &pkt, "out");
        printf("\n");

        //将每个包写入输出多媒体文件中
        ret = av_interleaved_write_frame(ofmt_ctx, &pkt);
        if (ret < 0) {
            fprintf(stderr, "Error muxing packet\n");
            break;
        }
        av_free_packet(&pkt);
    }
    free(dts_start_from);
    free(pts_start_from);

    av_write_trailer(ofmt_ctx);
end:

    avformat_close_input(&ifmt_ctx);

    /* close output */
    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
        avio_closep(&ofmt_ctx->pb);
    avformat_free_context(ofmt_ctx);

    if (ret < 0 && ret != AVERROR_EOF) {
        fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]){
    //四个参数：startime, endtime, srcfile, outfile
    if(argc < 5){
        fprintf(stderr, "Usage: \
                command startime, endtime, srcfile, outfile");
        return -1;
    }

    double startime = atoi(argv[1]);
    double endtime = atoi(argv[2]);
    cut_video(startime, endtime, argv[3], argv[4]);

    return 0;
}

