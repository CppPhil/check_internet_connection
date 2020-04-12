#include <cstdint>
#include <cstdio>
#include <cstring>

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <resolv.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "ping.hpp"

namespace cic {
namespace {
#define PACKETSIZE 64
struct packet {
  struct icmphdr hdr;
  char msg[PACKETSIZE - sizeof(struct icmphdr)];
};

unsigned short checksum(const void* b, int len) {
  const uint16_t* buf = static_cast<const uint16_t*>(b);
  unsigned int sum = 0;
  unsigned short result;

  for (sum = 0; len > 1; len -= 2)
    sum += *buf++;
  if (len == 1)
    sum += *(const unsigned char*) buf;
  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  result = ~sum;
  return result;
}
} // namespace

bool ping(const char* address) {
  const auto pid = getpid();
  struct protoent* proto = getprotobyname("ICMP");
  struct hostent* hname = gethostbyname(address);

  struct sockaddr_in addr_ping, *addr;

  memset(&addr_ping, 0, sizeof(addr_ping));
  addr_ping.sin_family = hname->h_addrtype;
  addr_ping.sin_port = 0;

  memcpy(&addr_ping.sin_addr.s_addr, &hname->h_addr,
         sizeof(addr_ping.sin_addr.s_addr));

  addr = &addr_ping;

  int sd = socket(PF_INET, SOCK_RAW, proto->p_proto);

  if (sd < 0)
    return false;

  const int val = 255;
  if (setsockopt(sd, SOL_IP, IP_TTL, &val, sizeof(val)) != 0) {
    close(sd);
    fprintf(stderr, "Could not set TTL to %d\n", val);
    return false;
  }

  if (fcntl(sd, F_SETFL, O_NONBLOCK) != 0) {
    close(sd);
    fprintf(stderr, "Could not set O_NONBLOCK\n");
    return false;
  }

  struct packet pckt;
  struct sockaddr_in r_addr;

  socklen_t len = static_cast<socklen_t>(sizeof(r_addr));

  memset(&pckt, 0, sizeof(pckt));
  pckt.hdr.type = ICMP_ECHO;
  pckt.hdr.un.echo.id = pid;

  size_t i;
  for (i = 0; i < sizeof(pckt.msg) - 1; i++)
    pckt.msg[i] = i + '0';
  pckt.msg[i] = 0;
  pckt.hdr.un.echo.sequence = 1;
  pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));

  if (sendto(sd, &pckt, sizeof(pckt), 0, (struct sockaddr*) addr, sizeof(*addr))
      <= 0) {
    close(sd);
    fprintf(stderr, "Could not send ICMP echo request\n");
    return false;
  }

  if (recvfrom(sd, &pckt, sizeof(pckt), 0, (struct sockaddr*) &r_addr, &len)
      > 0) {
    close(sd);
    fprintf(stderr, "Could not receive ICMP response\n");
    return false;
  }

  close(sd);
  return true;
}
} // namespace cic
