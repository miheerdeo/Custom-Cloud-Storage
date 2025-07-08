#include "httplib.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

int main() {
    httplib::Server server;

    // Log all requests for debugging
    server.set_logger([](const httplib::Request& req, const httplib::Response& res) {
        std::cout << req.method << " " << req.path << " -> " << res.status << std::endl;
    });

    // Serve the HTML form
    server.Get("/", [](const httplib::Request&, httplib::Response& res) {
        std::ifstream file("index.html");
        if (!file) {
            res.status = 500;
            res.set_content(" index.html not found", "text/plain");
            return;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        res.set_content(buffer.str(), "text/html");
    });

    // Handle photo upload
    server.Post("/upload", [](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_file("file")) {
            res.status = 400;
            res.set_content(" No file uploaded", "text/plain");
            return;
        }

        auto file = req.get_file_value("file");
        fs::create_directories("uploads");
        std::string save_path = "uploads/" + file.filename;

        std::ofstream out(save_path, std::ios::binary);
        out.write(file.content.c_str(), file.content.size());

        res.set_content(" Photo uploaded successfully!", "text/plain");
    });

    std::cout << "🚀 Server running at http://localhost:8080\n";
    server.listen("0.0.0.0", 8080);
}
