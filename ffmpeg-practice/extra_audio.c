#include <stdio.h>
#include <libavutil/log.h>
#include <libavformat/avformat.h>

#define ADTS_HEADER_LEN  7;
void adts_header(char *szAdtsHeader, int dataLen){

    int audio_object_type = 2;
    int sampling_frequency_index = 7;
    int channel_config = 2;

    int adtsLen = dataLen + 7;

    szAdtsHeader[0] = 0xff;         //syncword:0xfff                          高8bits
    szAdtsHeader[1] = 0xf0;         //syncword:0xfff                          低4bits
    szAdtsHeader[1] |= (0 << 3);    //MPEG Version:0 for MPEG-4,1 for MPEG-2  1bit
    szAdtsHeader[1] |= (0 << 1);    //Layer:0                                 2bits 
    szAdtsHeader[1] |= 1;           //protection absent:1                     1bit

    szAdtsHeader[2] = (audio_object_type - 1)<<6;            //profile:audio_object_type - 1                      2bits
    szAdtsHeader[2] |= (sampling_frequency_index & 0x0f)<<2; //sampling frequency index:sampling_frequency_index  4bits 
    szAdtsHeader[2] |= (0 << 1);                             //private bit:0                                      1bit
    szAdtsHeader[2] |= (channel_config & 0x04)>>2;           //channel configuration:channel_config               高1bit

    szAdtsHeader[3] = (channel_config & 0x03)<<6;     //channel configuration:channel_config      低2bits
    szAdtsHeader[3] |= (0 << 5);                      //original：0                               1bit
    szAdtsHeader[3] |= (0 << 4);                      //home：0                                   1bit
    szAdtsHeader[3] |= (0 << 3);                      //copyright id bit：0                       1bit  
    szAdtsHeader[3] |= (0 << 2);                      //copyright id start：0                     1bit
    szAdtsHeader[3] |= ((adtsLen & 0x1800) >> 11);           //frame length：value   高2bits

    szAdtsHeader[4] = (uint8_t)((adtsLen & 0x7f8) >> 3);     //frame length:value    中间8bits
    szAdtsHeader[5] = (uint8_t)((adtsLen & 0x7) << 5);       //frame length:value    低3bits
    szAdtsHeader[5] |= 0x1f;                                 //buffer fullness:0x7ff 高5bits
    szAdtsHeader[6] = 0xfc;
}

/**
 *argc:命令行中参数的个数
 *argv[]:参数值。其中第一个参数为我们程序的名字；后面的才是真正的参数 
 */
int main(int argc,char* argv[])
{   
    int ret;
    int len;
    int audio_index;//流的索引值：音频/视频 0/1

    char* src=NULL;//输入参数   
    char* dst=NULL;//输出参数

    AVPacket pkt;
    AVFormatContext *fmt_ctx=NULL;
        
    av_log_set_level(AV_LOG_INFO);
    
   // av_register_all(); 已弃用，无需注册了！
    
    //1.read tow params from console
    if(argc<3){
        av_log(NULL,AV_LOG_ERROR,"the count of params should be more than three!\n");
        return -1;
    }
    src=argv[1];
    dst=argv[2];
    //非空判断
    if(!src || !dst){
       av_log(NULL,AV_LOG_ERROR,"src or dst is null!\n");
       return -1;
    }

    //打开多媒体文件
    ret=avformat_open_input(&fmt_ctx,src,NULL,NULL);    
    if(ret<0){
        av_log(NULL,AV_LOG_ERROR,"Cant open file:%s\n",av_err2str(ret));
        return -1;
    }   
    //系统API:打开输出文件(目标文件)：普通的二进制文件。“wb”:可写的
    FILE* dst_fd= fopen(dst,"wb");
    if(!dst_fd){
        av_log(NULL,AV_LOG_ERROR,"Cant open out file!\n");
        avformat_close_input(&fmt_ctx);    
        return -1;
    }
    
    /**
    输出多媒体文件的Meta信息
    第三个参数：输入流/输出流 0/1
     */
    av_dump_format(fmt_ctx,0,src,0);    
    
    //2.get stream
    /**
    第二个参数：我们想获取的流的类型：AVMEDIA_TYPE_AUDIO 音频流
    第三个参数：我们要处理的流的索引号
    第四个参数：相关的索引号：比如我们音频对应的视频的索引号
    第五个参数：指定我们要处理流的编解码器
    第六个参数：flag
    */
    ret= av_find_best_stream(fmt_ctx,AVMEDIA_TYPE_AUDIO,-1,-1,NULL,0);
    if(ret<0){
        av_log(NULL,AV_LOG_ERROR,"Cant find the best stream!\n");
        avformat_close_input(&fmt_ctx);
        //关闭输出文件
        fclose(dst_fd);
        return -1;    
    }
    audio_index=ret;
    
    av_init_packet(&pkt);
    
    //从多媒体文件中获取每一个包
    while(av_read_frame(fmt_ctx,&pkt)>=0){
        if(pkt.stream_index==audio_index){
            //是我们想要处理的音频数据
            
            //添加adts header 信息
            char adts_header_buf[7];
            adts_header(adts_header_buf, pkt.size);
            fwrite(adts_header_buf, 1, 7, dst_fd);

           // adts_write_packet(fmt_ctx,&pkt); 
            //3.写到目标文件中  write  audio data to aac file
            len=fwrite(pkt.data,1,pkt.size,dst_fd);
            if(len!=pkt.size){
                av_log(NULL,AV_LOG_WARNING,"warning!length of data is not equal size of pkt!\n");
            }
        }       
        //如果不是则释放这个空间
        av_packet_unref(&pkt);
    }
 
    //关闭多媒体文件
    avformat_close_input(&fmt_ctx);
    //关闭目标文件    
    if(dst_fd){
        fclose(dst_fd);
    }
    return 0;
}
