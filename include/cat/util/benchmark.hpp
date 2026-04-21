#include <cat/config.hpp>
#include <cat/util/chrono.hpp>
#include <string_view>			// IWYU pragma: keep
#include <unordered_map>		// IWYU pragma: keep
#include <vector>			// IWYU pragma: keep

// @file benchmark.hpp defines the BenchMarker struct, used to benchmark
// 	a the duration of a scope in the code
//
// note that all benchmarking features are locked behind the ENABLE_BENCHAMRK macro

#if ENABLE_BENCHMARK

namespace cat
{

struct BenchMarker
{
	BenchMarker(const std::string_view& label);
	~BenchMarker();

	static void display(bool convert_to_seconds = true);
private:
	const std::string_view m_label;
	const millis_t m_start_time;

	static std::unordered_map<std::string_view, std::vector<millis_t>> s_library;
};

}

#define CAT_BENCH_SCOPE(label, varname) cat::BenchMarker varname{label} 
#define CAT_BENCH_DISPLAY(convert) cat::BenchMarker::display(convert)

#else

#define CAT_BENCH_SCOPE(label, varname)
#define CAT_BENCH_DISPLAY(convert)

#endif

