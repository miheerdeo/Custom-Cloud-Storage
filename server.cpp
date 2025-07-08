#include "httplib.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
namespace fs = std::filesystem;

int main() {
    httplib::Server server; // Instance of Http server for get and post request
    /*
       server.set_logger:- functionality provided by http lib for logging each request and response to the server.
       Implementation:-
       It takes lambda as callback and every time the request is received to the server the http method, path of data saved and status code is printed
       HTTP status codes used:-
       1) 200:- Request succeeded.
       2) 500:- Internal Server errors
       3) 400:- Bad Request
    */

    server.set_logger([](const httplib::Request& req, const httplib::Response& res) {
        cout << req.method << " " << req.path << " -> " << res.status << endl;
    });

    // Serve the HTML form
    /*
      server.Get:- It is the GET request of the HTTP server. GET is used to receive the data from the server, in this case it is the index.html form to upload the photos
      Implementation:-
      Lambda function takes the request (URL with '/') and response is sent as index.html file. It reads the HTML file into buffer and then sends the content.
      "text/html" tells browser to treat it as an HTML page.
    */
    server.Get("/", [](const httplib::Request&, httplib::Response& res) {
        ifstream file("index.html");
        if (!file) {
            res.status = 500;
            res.set_content("index.html not found", "text/plain");
            return;
        }

        stringstream buffer;
        buffer << file.rdbuf();
        res.set_content(buffer.str(), "text/html");
    });

    // Handle photo upload
    /*
      server.Post:- It is the POST request handler to receive files uploaded through the form.
      Implementation:-
       Lamba funnction that checks the uploaded photo file handles if it does not exits, and gets its metadata and save it to the uploads 
       directory,opens a output stream and saves the photo into disk in binary mode.
    */
    server.Post("/upload", [](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_file("file")) {
            res.status = 400;
            res.set_content("No file uploaded", "text/plain");
            return;
        }

        auto file = req.get_file_value("file");
        fs::create_directories("uploads");
        string save_path = "uploads/" + file.filename;

        ofstream out(save_path, ios::binary);
        out.write(file.content.c_str(), file.content.size());

        res.set_content("Photo uploaded successfully!", "text/plain");
    });
    //0.0.0.0 allows to bind on all the network interfaces.
    cout << "🚀 Server running at port 8080";
    server.listen("0.0.0.0", 8080);
}
