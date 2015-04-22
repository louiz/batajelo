#include <network/remote_client_base.hpp>
#include <network/tcp_socket.hpp>

template <>
unsigned long int RemoteClientBase<TCPSocket>::clients_number = 0;
