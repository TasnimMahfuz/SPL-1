/*sample call for a client to connect
to a particular server, i.e. port 3490*/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    /*

    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;//for pointing to resuts.

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;//ipv4 and ipv6
    hints.ai_socktype = SOCK_STREAM;//tcp stream sockets.


    status = getaddrinf("www.example.net",
                        "3490",
                        &hints,
                        &servinfo);

     //servinfo should point to
       // a linked list of 1 or more
       //struct addrinfos

        */

        struct addrinfo hints,*res,*p;
        int status;

        char ipstr[INET6_ADDRSTRLEN];

        if(argc != 2){
            fprintf(stderr,"usage: showip hostname.\n");
            return 1;
        }

        memset(&hints,0,sizeof hints);

        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;//what does this mean?

        if((status = getaddrinfo(argv[1],NULL, &hints,&res)) != 0)
        {
            fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(status));
            return 2;
        }

        printf("IP addresses for %s\n\n",argv[1]);

        for(p = res; p != NULL; p = p -> ai_next)
        {
            void *addr;
            char *ipver;

            if(p -> ai_family == AF_INET)
            {
                //ipv4;

                struct sockaddr_in *ipv4 = (struct sockaddr_in*)p ->ai_addr;
                addr = &(ipv4->sin_addr);
                ipver = "IPv4";
            }else
            {
                //IPv6

                struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) p -> ai_addr;
                addr = &(ipv6 -> sin6_addr);
                ipver = "IPv6";
            }

            //IP to a string conversion.

            inet_ntop(p -> ai_family, addr, ipstr, sizeof ipstr);//inet_ntop ki kore?
            printf(" %s: %s\n", ipver, ipstr);
        }

        freeaddrinfo(res);

        return 0;

        /*
        Fastly error: unknown domain: 151.101.130.133. Please check that this domain has been added to a service.

        Details: cache-hkg17935-HKG

        This is what I get when I use the found IP address
        In the search bar of a browser.

        But, this is a valid ip for sure. So, what is the problem here?

        */

        /*
            IP addresses for www.youtube.com

 IPv4: 216.58.200.174
 IPv4: 142.250.193.238
 IPv4: 142.250.193.78
 IPv4: 142.250.193.14
 IPv4: 172.217.166.14
 IPv4: 142.250.206.110
 IPv4: 142.250.192.174
 IPv4: 142.250.193.206
 IPv4: 142.250.192.206
 IPv4: 142.250.193.46
 IPv4: 172.217.167.206
 IPv4: 142.250.192.238
 IPv4: 172.217.167.238
 IPv4: 216.58.196.110
 IPv4: 142.250.77.238
 IPv4: 142.250.77.206
 IPv6: 2404:6800:4007:823::200e
 IPv6: 2404:6800:4007:814::200e
 IPv6: 2404:6800:4007:822::200e
 IPv6: 2404:6800:4007:813::200e

 But these addresses lead me to www.google.com, not to www.youtube.com.

 So, the code works, but not quite in the way I expect it to be.
 Still, this is beautiful
 !

        */
}







