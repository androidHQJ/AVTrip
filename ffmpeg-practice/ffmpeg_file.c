#include <libavformat/avformat.h>

int main(int argc,char* argv[])
{
    int ret;
    //rename file
    ret= avpriv_io_move("111.txt","222.txt");    
    if(ret<0){
        av_log(NULL,AV_LOG_ERROR,"Fail to rename!\n");
        return -1;
    }else{
        av_log(NULL,AV_LOG_INFO,"Rename Success!\n");
        return 0;
    }

    //delete file
    ret=avpriv_io_delete("./mytestfile.txt");
    if(ret<0){
        av_log(NULL,AV_LOG_ERROR,"Failed to delete file mytestfile.txt\n");
        return -1;
    }else{
        av_log(NULL,AV_LOG_INFO,"Delete Success!\n");
        return 0;
    }
}
