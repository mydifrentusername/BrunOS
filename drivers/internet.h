#ifndef DRIVERS_INTERNET_H
#define DRIVERS_INTERNET_H

//all definitions
void driver_initialize(void);
void driver_cleanup(void);
void transmit_packet(void *packet, int length);
void receive_packet(void *buffer, int buffer_size);
void set_ip_address(const char *ip_address);
void set_mac_address(const char *mac_address);
void interrupt_service_routine(void);
void get_connection_status(void);
void get_packet_statistics(void *statistics);
void establish_connection(const char *server_address, int port);
void maintain_connection(void);
void terminate_connection(void);
void handle_error(int error_code);


#endif