
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/util/high_resolution_clock.hpp>
#include <hpx/include/parallel_algorithm.hpp>
//#include <hpx/parallel/segmented_algorithms/for_each.hpp>
#include <hpx/parallel/algorithms/for_each.hpp>
//#include <hpx/include/parallel_executor_parameters.hpp>
#include <hpx/include/iostreams.hpp>

int hpx_main(int argc, char **argv)
{

	//hpx::parallel::dynamic_chunk_size dcs(100);//(hpx::parallel::task).with(dcs)
	auto r=boost::irange(0, 100);
	double t_a=double(0);

	boost::uint64_t t=hpx::util::high_resolution_clock::now();



	hpx::parallel::for_each_dynamic(hpx::parallel::par, r.begin(), r.end(), t_a, [](std::size_t d){ 
					d=d+1; 
				});

	hpx::parallel::for_each_dynamic(hpx::parallel::par, r.begin(), r.end(), t_a, [](std::size_t d){ 
					for(int i=0; i<10000; ++i) {
						d=d*2; 
						d=d-2;}
				});

	hpx::parallel::for_each_dynamic(hpx::parallel::par, r.begin(), r.end(), t_a, [](std::size_t d){ 
					d=d+1; 
					for(int i=0; i<1000000000; ++i) {
						d=d*2; 
						d=d-2;}
				});



	boost::uint64_t elapsed=hpx::util::high_resolution_clock::now()-t;
	std::cout<<(boost::format("%.14g")%(elapsed/ 1e9)) <<std::flush<<std::endl;
	
	return hpx::finalize();
}



int main(int argc, char **argv){
	return hpx::init(argc,argv);
}
