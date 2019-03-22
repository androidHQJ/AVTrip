#include <libavutil/log.h>
#include <libavformat/avformat.h>

int main(int argc,char* argv[])
{
    int ret;
    av_log_set_level(AV_LOG_INFO); 
    
    AVIODirContext *ctx=NULL;
    AVIODirEntry *entry =NULL;

    //打开目录
    ret=avio_open_dir(&ctx,"./",NULL);
    if(ret<0){
        av_log(NULL,AV_LOG_ERROR,"Cant open dir:%s\n",av_err2str(ret));
        return -1;
    }
    
    //访问目录中的每一项
    while(1){
        ret=avio_read_dir(ctx,&entry);
        if(ret<0){
            av_log(NULL,AV_LOG_ERROR,"Cant read dir:%s\n",av_err2str(ret));
            
            goto _fail;
         }
        //如果entry为空，即读到目录的最后一项则退出循环
        if(!entry){
            break;
        }
        //打印目录信息
       /**
      "%12"PRId64: 12位的64位整数
       */
        av_log(NULL,AV_LOG_INFO,"%12"PRId64" %s \n",
               entry->size,
               entry->name);
        //释放回收entry
        avio_free_directory_entry(&entry);
    }

_fail:
    avio_close_dir(&ctx);

    return 0;

}
