#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cerrno>
#include <vector>

#define BUFFER_SIZE 4096

void set_non_blocking(int sockfd)
{
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

void handle_error(const std::string &msg)
{
    perror(msg.c_str());
    exit(EXIT_FAILURE);
}

void forward_data(int source, int target)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(source, buffer, sizeof(buffer))) > 0)
    {
        if (write(target, buffer, bytes_read) < 0)
        {
            handle_error("Error writing to target");
        }
    }

    if (bytes_read < 0 && errno != EAGAIN)
    {
        handle_error("Error reading from source");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <server_port> <target_ip> <target_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_fd, target_fd;
    struct sockaddr_in server_addr, target_addr;

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        handle_error("Socket creation failed");
    }

    // Configure socket options
    set_non_blocking(server_fd);

    // Bind server socket
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        handle_error("Bind failed");
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) < 0)
    {
        handle_error("Listen failed");
    }

    // Create target socket
    if ((target_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        handle_error("Target socket creation failed");
    }

    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(atoi(argv[3]));
    if (inet_pton(AF_INET, argv[2], &target_addr.sin_addr) <= 0)
    {
        handle_error("Invalid target IP address");
    }

    // Connect to target
    if (connect(target_fd, (struct sockaddr *)&target_addr, sizeof(target_addr)) < 0)
    {
        handle_error("Connection to target failed");
    }

    // Accept incoming connections and forward data
    while (true)
    {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0)
        {
            handle_error("Accept failed");
        }

        // Forward data between client and target
        forward_data(client_fd, target_fd);
        close(client_fd);
    }
    close(server_fd);
    close(target_fd);
    return 0;
}

