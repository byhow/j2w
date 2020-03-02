#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct games {
    char* type;
    char* game_id;
    char* home_team;
    char* away_team;
    char* week;
    char* season;
    char* home_score;
    char* away_score;
};

static struct games game_table[400];

int parsedb(char* data_base_f) {
    FILE* fp;
    char line[200];
    fp = fopen(data_base_f, "r");
    if (fp==NULL) {
        printf("Fail to open the file %s \n", data_base_f);
        return -1;
    }

    int size = -1;
    while (fgets(line, 255, (FILE*)fp)) {
        if (line[0]=='\n' || size==-1) { 
            printf("at -1");
            size++;
            continue; 
        }
        char* type = strtok(line, ",");
        char* game_id = strtok(NULL, ",");
        char* home_team = strtok(NULL, ",");
        char* away_team = strtok(NULL, ",");
        char* week = strtok(NULL, ",");
        char* season = strtok(NULL, ",");
        char* home_score = strtok(NULL, ",");
        char* away_score = strtok(NULL, ",");
        away_score[strlen(away_score)-1] = '\0';
        game_table[size].type = type;
        game_table[size].game_id = game_id;
        game_table[size].home_team = home_team;
        game_table[size].away_team = away_team;
        game_table[size].week = week;
        game_table[size].season = season;
        game_table[size].home_score = home_score;
        game_table[size].away_score = away_score;
        size++;
    } 
    fclose(fp);
    return 0;
}


int main(int argc, char* argv[]) { 
    
    int port = atoi(argv[2]);
    char* data_base = argv[1];
    printf("Starting server on database: %s, on port %d\n", data_base, port);
    parsedb(data_base);
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int optval = 1;
    int server_fd, new_socket, valread;
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        while (1)
        {
            char recv_buffer[257];
            memset(recv_buffer,0, 257);
            valread = recv(new_socket, recv_buffer, 257, 0);
            int buf_len = (int) (recv_buffer[0]);
            char* buf = recv_buffer+1;
            printf("%s\n", buf);
            printf("%d\n", buf_len);
            send(new_socket, buf, 257, 0);
        }
        
    }
    return 0; 
}