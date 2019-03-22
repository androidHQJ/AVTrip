#include <libavutil/log.h>
#include <libavformat/avformat.h>

int main(int argc,char* argv[])
{   
    int ret;
    AVFormatContext *fmt_ctx=NULL;

    av_log_set_level(AV_LOG_INFO);
    
   // av_register_all(); 已弃用，无需注册了！

    ret=avformat_open_input(&fmt_ctx,"./test.mp4",NULL,NULL);    
    if(ret<0){
        av_log(NULL,AV_LOG_ERROR,"Cant open file:%s\n",av_err2str(ret));
        return -1;
     }

    /**
    第三个参数：输入流/输出流 0/1
     */
    av_dump_format(fmt_ctx,0,"./test.mp4",0);    
    
    avformat_close_input(&fmt_ctx);    

    return 0;
}
