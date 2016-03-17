//  Copyright (c) 2014 Grant Mercer
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/util/high_resolution_clock.hpp>
#include <hpx/include/parallel_algorithm.hpp>
#include <hpx/include/parallel_executor_parameters.hpp>
#include <hpx/include/iostreams.hpp>

#include <boost/cstdint.hpp>
#include <boost/format.hpp>
#include <boost/range/functions.hpp>

#include <stdexcept>

///////////////////////////////////////////////////////////////////////////////
int delay = 1000;
int test_count = 100;
int chunk_size = 0;
int num_overlapping_loops = 0;



void measure_parallel_foreach(std::size_t size)
{
    std::vector<std::size_t> data_representation(size);
    std::iota(boost::begin(data_representation),
        boost::end(data_representation),
        std::rand());

    // create executor parameters object
    hpx::parallel::static_chunk_size cs(chunk_size);

    // invoke parallel for_each
    hpx::parallel::for_each(hpx::parallel::par.with(cs),
        boost::begin(data_representation),
        boost::end(data_representation),
        [](std::size_t d) {
            d=d+1; 
        });

    hpx::parallel::for_each(hpx::parallel::par.with(cs),
        boost::begin(data_representation),
        boost::end(data_representation),
        [](std::size_t d) {
            for(int i=0; i<10000; ++i) {
		d=d*2; 
		d=d-2;}
        });

    hpx::parallel::for_each(hpx::parallel::par.with(cs),
        boost::begin(data_representation),
        boost::end(data_representation),
        [](std::size_t d) {
            	d=d+1; 
		for(int i=0; i<1000000000; ++i) {
			d=d*2; 
			d=d-2;}
        });
}


int hpx_main(boost::program_options::variables_map& vm)
{
    //pull values from cmd
    std::size_t vector_size = vm["vector_size"].as<std::size_t>();
    chunk_size = vm["chunk_size"].as<int>();

    //verify that input is within domain of program
    if(test_count == 0 || test_count < 0) {
        hpx::cout << "test_count cannot be zero or negative...\n" << hpx::flush;
    } else if (delay < 0) {
        hpx::cout << "delay cannot be a negative number...\n" << hpx::flush;
    } else {

	boost::uint64_t t=hpx::util::high_resolution_clock::now();
	measure_parallel_foreach(vector_size);
	boost::uint64_t elapsed=hpx::util::high_resolution_clock::now()-t;

        std::cout<<(boost::format("%.14g")%(elapsed/ 1e9)) <<std::flush<<std::endl;
    }

    return hpx::finalize();
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    //initialize program
    std::vector<std::string> cfg;
    cfg.push_back("hpx.os_threads=" +
        boost::lexical_cast<std::string>(hpx::threads::hardware_concurrency()));
    boost::program_options::options_description cmdline(
        "usage: " HPX_APPLICATION_STRING " [options]");

    cmdline.add_options()
        ( "vector_size"
        , boost::program_options::value<std::size_t>()->default_value(10)
        , "size of vector")

        ("chunk_size"
        , boost::program_options::value<int>()->default_value(0)
        , "number of iterations to combine while parallelization")
        ;

    return hpx::init(cmdline, argc, argv, cfg);
}

