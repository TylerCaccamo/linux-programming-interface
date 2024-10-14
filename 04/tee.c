#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <getopt.h>
#include <fcntl.h>

#define EXT_SUCCESS 0
#define BUFFER_SIZE 1024

int main(int argc, char** argv) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int fd;
    int opt;
    bool append_mode = false;

    bool input_from_pipe = !isatty(STDIN_FILENO);

    while ((opt = getopt(argc, argv, "a")) != -1) {
	switch (opt) {
	    case 'a':
		append_mode = true;
		break;
	    default:
		fprintf(stderr, "Usage: %s [-a] [file]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
    }

    printf("APPEND_MODE = %d\n", append_mode);
    char *file_name = argv[optind];

    if (argc == 1) {
	// Case of no arguments passed should turn into shell like mode and display input back to terminal
	// While bytes_read > 0 write to stdout
	while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
	    // Try to write to standard out from the buffer with bytes read amount 
	    if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read) {
		perror("Error writing to stdout");
		exit(EXIT_FAILURE);
	    }
	}

	if (bytes_read == -1) {
	    perror("Error reading from stdin");
	    exit(EXIT_FAILURE);
	}
	
    } else if (argc > 1) {
	// Same situation but this time we need to open the file passed and write to the file

	if (input_from_pipe == 0) {
	    if (append_mode == 1) {
		fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);

		if (fd == -1) {
		    perror("Error opening file");
		    exit(EXIT_FAILURE);
		}

		while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
		    // Try to write to standard out from the buffer with bytes read amount 
		    if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read) {
			perror("Error writing to stdout");
			exit(EXIT_FAILURE);
		    }

		    if (write(fd, buffer, bytes_read) != bytes_read) {
			perror("Error writing to file");
			exit(EXIT_FAILURE);
		    }
		}

		if (close(fd) == -1) {
		    perror("Error closing file");
		}
	    } else {


		while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {

		    fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);

		    if (fd == -1) {
			perror("Error opening file");
			exit(EXIT_FAILURE);
		    }

		    // Try to write to standard out from the buffer with bytes read amount 
		    if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read) {
			perror("Error writing to stdout");
			exit(EXIT_FAILURE);
		    }

		    if (write(fd, buffer, bytes_read) != bytes_read) {
			perror("Error writing to file");
			exit(EXIT_FAILURE);
		    }

		    if (close(fd) == -1) {
			perror("Error closing file");
		    }

		}

		if (bytes_read == -1) {
		    perror("Error reading from stdin");
		    exit(EXIT_FAILURE);
		}
	    }

	} else {
	    if (append_mode == 1) {
		fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);

		if (fd == -1) {
		    perror("Error opening file");
		    exit(EXIT_FAILURE);
		}

		while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
		    // Try to write to standard out from the buffer with bytes read amount 
		    if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read) {
			perror("Error writing to stdout");
			exit(EXIT_FAILURE);
		    }

		    if (write(fd, buffer, bytes_read) != bytes_read) {
			perror("Error writing to file");
			exit(EXIT_FAILURE);
		    }
		}

		if (close(fd) == -1) {
		    perror("Error closing file");
		}

	    } else {

		fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);

		if (fd == -1) {
		    perror("Error opening file");
		    exit(EXIT_FAILURE);
		}

		if ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
		    if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read) {
			    perror("Error writing to stdout");
			    exit(EXIT_FAILURE);
		    }
		    if (write(fd, buffer, bytes_read) != bytes_read) {
			    perror("Error writing to file");
			    exit(EXIT_FAILURE);
		    }
		}
	    }
	}
    }
}

