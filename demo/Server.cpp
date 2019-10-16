
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <RCF/RCF.hpp>
#include <RCF/Win32NamedPipeEndpoint.hpp>
#include <RCF/Win32NamedPipeServerTransport.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include "MyService.hpp"

class MyServiceImpl
{
public:
    // Reverses the order of strings in the vector.
    void reverse(std::vector<std::string> &v)
    {
        std::cout << "Reversing a vector of strings...\n";
        std::vector<std::string> w;
        std::copy(v.rbegin(), v.rend(), std::back_inserter(w));
        v.swap(w);
    }

	void sayHello()
	{
		std::cout << "SayHello\n";
	}
};

int main()
{
	try
	{
		RCF::init();

		boost::shared_ptr<RCF::Win32NamedPipeServerTransport> pipeserver = 
			boost::make_shared<RCF::Win32NamedPipeServerTransport>("123");

		RCF::ServerTransportPtr servertransport = RCF::ServerTransportPtr(pipeserver);

		boost::shared_ptr<RCF::RcfServer> rcfserver = boost::make_shared<RCF::RcfServer>(servertransport);
		MyServiceImpl myServiceImpl;
		rcfserver->bind<MyService>(myServiceImpl);
		rcfserver->start();
	 

		// Start a TCP server on port 50001, and expose MyServiceImpl.
		
		RCF::RcfServer server(RCF::TcpEndpoint("0.0.0.0", 50001));
		server.bind<MyService>(myServiceImpl);
		server.start();

		 
		std::cout << "Press Enter to exit..." << std::endl;
		std::cin.get();
	}
	catch (const RCF::Exception & e)
	{
		std::cout << "Caught exception:\n";
		std::cout << e.getError().getErrorString() << std::endl;
		std::cin.get();
		return 1;
	}
    

    return 0;
}
