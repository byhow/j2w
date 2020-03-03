#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* types[400];
char* game_ids[400];
char* home_teams[400];
char* away_teams[400];
char* weeks[400];
char* seasons[400];
char* home_scores[400];
char* away_scores[400];
static int size = -1;

int parsedb(char* data_base_f) {
    FILE* fp;
    char line[200];
    fp = fopen(data_base_f, "r");
    if (fp==NULL) {
        printf("Fail to open the file %s \n", data_base_f);
        return -1;
    }

    while (fgets(line, 255, (FILE*)fp)) {
        if (line[0]=='\n' || size==-1) {
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

        
        types[size] = malloc(strlen(type));
        game_ids[size] = malloc(strlen(game_id));
        home_teams[size] = malloc(strlen(home_team));
        away_teams[size] = malloc(strlen(away_team));
        weeks[size] = malloc(strlen(week));
        seasons[size] = malloc(strlen(season));
        home_scores[size] = malloc(strlen(home_score));
        away_scores[size] = malloc(strlen(away_score));

        strcpy(types[size], type);
        strcpy(game_ids[size], game_id);
        strcpy(home_teams[size], home_team);
        strcpy(away_teams[size], away_team);
        strcpy(weeks[size], week);
        strcpy(seasons[size], season);
        strcpy(home_scores[size], home_score);
        strcpy(away_scores[size], away_score);
        size++;
        
    } 
    fclose(fp);
    return 0;
}


char* search(char* game_id, char* field) {
    int i = 0;
    for (; i < size; i++) {
        if (!strcmp(game_ids[i], game_id)) {
            if(!strcmp(field, "type")) {
                return types[i];
            } else if (!strcmp(field, "game_id")) {
                return game_ids[i];
            } else if (!strcmp(field, "home_team")) {
                return home_teams[i];
            } else if (!strcmp(field, "away_team")) {
                return away_teams[i];
            } else if (!strcmp(field, "week")) {
                return weeks[i];
            } else if (!strcmp(field, "season")) {
                return seasons[i];
            } else if (!strcmp(field, "home_score")) {
                return home_scores[i];
            } else if (!strcmp(field, "away_score")) {
                return away_scores[i];
            }
        }
    }
    return "unknown";
}

int main(int argc, char* argv[]) {
    int port = atoi(argv[2]);
    char* data_base = argv[1];
    printf("server started\n");
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
            if (!strncmp(buf, "quit", 4)) {
                send(new_socket, buf, strlen(buf), 0);
                return 0;
            }
            // parse buffer
            char* game_id = strtok(buf, " ");
            char* field = strtok(NULL, "\n");
            printf("%s %s\n", game_id, field);
            char* resp = search(game_id, field);
            
            // return result
            int len = strlen(resp);
            unsigned char length = len & 0xFF;
            unsigned char query[257];
            query[0] = length;
            query[1] = '\0';
            strcat(query, resp);
            send(new_socket, query, strlen(query), 0);
        }
        
    }
    
    return 0; 
}