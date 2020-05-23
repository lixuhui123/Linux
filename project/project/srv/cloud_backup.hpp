#include<cstdio>
#include<string>
#include<vector>
#include<fstream>
#include<iostream>
#include<boost/filesystem.hpp>
#include<unordered_map>
#include<zlib.h>
#include<pthread.h>
#include"httplib.h"
namespace _cloud_sys
{
    class FileUtil
    {
        public:
            //从文件中读取所有数据
            static bool  Read(const std::string &name,std::string *body)
            {
                std::ifstream fs(name,std::ios::binary);
                if(fs.is_open()==false)
                {
                    std::cout<<"opne file"<<name <<"failed"<<std::endl;
                    return false;
                }
                //获取文件大小
                int64_t fsize=boost::filesystem::file_size(name);
                body->resize(fsize);
                //read(char * buf,size_t size)
                fs.read(&(*body)[0],fsize);//body是指针得先解运用才能使用[]重载
                if(fs.good()==false)
                {
                    std::cout<<"file"<<name<<"read data faild!\n";
                    return false;
                }
                fs.close();
                return true;
            }
            //向文件中写入数据
            static bool Write(const std::string &name,const std::string &body)
            {
                //输出流 ofstream默认打开文件的时候会清空原有的内容，当前策略是覆盖写入 
                std::ofstream ofs(name,std::ios::binary);
                if(ofs.is_open()==false)
                {
                    std::cout<<"open file"<<name<<"is failed"<<std::endl;
                    return false;
                }
                ofs.write(&body[0],body.size());
                if(ofs.good()==false)
                {
                    std::cout<<"file"<<name<<"write failed"<<std::endl;
                    return false;
                }
                ofs.close();
                return true;

            }

    };
    class CompressUtil
    {
        public:
            //文件压缩-源文件名称-压缩包名称
            static bool Compress(const std::string &src,const std::string &dst)
            {
                std::string body;
                FileUtil::Read(src,&body);
                gzFile gf=gzopen(dst.c_str(),"wb");//打开压缩包
                if(gf==NULL)
                {
                    std::cout<<"open file"<<dst<<"failed\n";
                    return false;
                }
                size_t wlen=0;
                while(wlen<body.size())
                {
                    int ret=gzwrite(gf,&body[wlen],body.size()-wlen);
                    if(ret==0)
                    {
                            std::cout<<"file"<<dst<<"writing compress failed\n";
                            return false;
                    }
                    wlen+=ret;
                }
                gzclose(gf);
                return true;
            }
            //文件解压缩-压缩包名称-存储路径
            static bool UnCompress(const std::string &src,const std::string &dst)
            {
                std::ofstream ofs(dst,std::ios::binary);
                if(ofs.is_open()==false)
                {
                    std::cout<<"file"<<dst<<"open failed"<<std::endl;
                    return false;
                }
                gzFile gf=gzopen(src.c_str(),"rb");
                if(gf==NULL)
                {
                    std::cout<<"open fail"<<src<<"opne compress file failed\n";
                    ofs.close();
                    return false;
                }
                char tmp[4096]={0};
                int ret=0;
                while((ret=gzread(gf,tmp,4096))>0)
                {
                    ofs.write(tmp,ret);
                }
                ofs.close();
                gzclose(gf);

                return true;
            }
            
    };
 /*   class NonHotCompress
    {
        private:
            //判断一个文件是否是热点文件
            bool FileIsHot(const std::string name);
            std::string _gz_dir;//压缩后的文件存储路径
    };

    class Server
    {
        public:
            bool Start();//启动网络通信模块
        private:
            //文件上传请求
            static void Upload(const httplib::Request & req,httplib::Request &rsp); 
            //文件列表请求
            static void List(const httplib::Request & req,httplib::Request &rsp);
            //文件下载处理回调函数
            static void Download(const httplib::Request & req,httplib::Request &rsp); 
        private:
            std::string _file_dir;//文件上传备份路径
            httplib::Server _server;


    };

    class DataManager
    {

        public:
            //判断文件是否存在
            bool Exists(const std::string &name);
            //判断文件是否已经压缩
            bool IsCompress(const std::string &name);
            //获取未压缩的文件列表
            bool NOnCompressList(std::vector<std::string>* list );
            //插入/更新数据
            bool Insert(const std::string &src,const std::string &dst);
            //获取所有文件名称
            bool GetAllName(std::vector<std::string>*list);
            //数据改变后的持久化存储
            bool Storage();
            //启动时初始化加载原有数据
            bool InitLoad(); 
        private:
            std::string _back_file;
            std::unordered_map<std::string,std::string> _file_list; //文件的持久化存储
             pthread_rwlock_t _rwlock;
    };*/
}
