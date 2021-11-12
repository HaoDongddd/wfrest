//
// Created by Chanchan on 11/8/21.
//

#include "HttpServer.h"
#include "HttpMsg.h"
#include "workflow/WFFacilities.h"
#include <csignal>

using namespace wfrest;

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo)
{
    wait_group.done();
}

int main()
{
    signal(SIGINT, sig_handler);

    HttpServer svr;

    svr.Post("/post", [](HttpReq *req, HttpResp *resp)
    {
        std::string host = req->header("Host");
        std::string content_type = req->header("Content-Type");
        if(req->has_header("User-Agent"))
        {
            fprintf(stderr, "Has User-Agent...");
        }
        resp->String(host + " " + content_type + "\n");
    });


    if (svr.start(9001) == 0)
    {
        wait_group.wait();
        svr.stop();
    } else
    {
        fprintf(stderr, "Cannot start server");
        exit(1);
    }
    return 0;
}