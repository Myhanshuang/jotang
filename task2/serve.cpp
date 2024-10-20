#include <httplib.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

using namespace std;
using namespace httplib;

void list_directory(const char *path) // 手写 ls
{
    DIR *dir; // 来自 #include<dirent.h>
    // 声明一个指针，用于指向目录流
    struct dirent *entry;
    // 该目录下一个成员的部分信息
    // 来自 #include<dirent.h>

    // 来自 #include<sys/stat.h>
    // 存储某个成员的所有详细信息
    struct stat statbuf;

    if (!(dir = opendir(path)))
    { // 访问失败会返回0
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    auto fp = fopen("direction.txt", "w");
    while ((entry = readdir(dir)) != NULL)
    { // 抓取该目录下的文件
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        // 等价于sprintf, 只不过更加安全

        // stat 获取目录下文件的详细信息
        if (stat(full_path, &statbuf) == -1)
        { // 获取失败
            perror("stat");
            continue;
        }
        if (entry->d_name[0] == '.')
            continue;
        int n = strlen(entry->d_name);
        for (int i = 0; i < n; i++)
            fprintf(fp, "%c", entry->d_name[i]); // 文件名
        fprintf(fp, "\n");
    }
    fclose(fp);
    closedir(dir); // 关闭文件流
}
void handle_request(const Request &req, Response &res)
{
    const string path = req.path; // 获取请求的路径

    // 构建文件路径
    string file_path = "./public" + path;
    bool f = 0;
    for (int i = 2; i < file_path.size(); i++)
        if (file_path[i] == '.')
            f = 1;
    if (!f) // 文件夹
    {
        chdir(file_path.c_str());
        list_directory("."); // 获取文件夹里面的文件目录
        chdir("..");
        stringstream buffer;
        string output = file_path + "direction.txt";
        ifstream iffile(output.c_str(), ios::binary);
        buffer << iffile.rdbuf();
        res.set_content(buffer.str(), "text/plain");
    }
    else // 文件
    {
        // 读取
        ifstream ifile(file_path.c_str(), ios::binary);
        stringstream buffer;
        buffer << ifile.rdbuf();

        // 根据文件扩展名设置Content-Type
        auto ext = file_path.substr(file_path.find_last_of("."));
        if (ext == ".html")
        {
            res.set_header("Content-Type", "text/html");
        }
        else if (ext == ".css")
        {
            res.set_header("Content-Type", "text/css");
        }
        else if (ext == ".js")
        {
            res.set_header("Content-Type", "application/javascript");
        }
        else if (ext == ".jpg" || ext == ".jpeg")
        {
            res.set_header("Content-Type", "image/jpeg");
        }
        else if (ext == ".png")
        {
            res.set_header("Content-Type", "image/png");
        }
        else
        {
            res.set_header("Content-Type", "text/plain");
        }
        // 设置响应内容
        res.set_content(buffer.str(), "text/plain");
    }
}

int main()
{
    Server svr;
    svr.Get("/(.*)", handle_request);

    svr.listen("localhost", 8080);

    return 0;
}