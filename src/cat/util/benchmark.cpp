#if BENCHMARK_ENABLED

#include "cat/util/chrono.hpp"
#include <cat/util/benchmark.hpp>

#include <cat/util/logger.hpp>
#include <cmath>
#include <numeric>


std::unordered_map<std::string_view, std::vector<millis_t>> cat::BenchMarker::s_library;

cat::BenchMarker::BenchMarker(const std::string_view& label)
	: m_label(label)
	, m_start_time(cat::chrono::current_millis())
{}

cat::BenchMarker::~BenchMarker()
{
	millis_t duration = chrono::current_millis() - m_start_time;
	s_library[m_label].push_back(duration);
}

static f64 _variance(const std::vector<millis_t>& data)
{
	if(data.empty()) return 0;

	const f64 size = static_cast<f64>(data.size());
	const f64 mean = std::accumulate(
		data.begin(),
		data.end(),
		0.0
	) / size;

	auto variance_func = [&mean, &size](f64 accumulator, const millis_t& val)
	{
		f64 dval = static_cast<f64>(val);
		return accumulator + ((dval - mean)*(dval-mean) / (size - 1));
	};

	return std::accumulate(
		data.begin(),
		data.end(),
		0.0,
		variance_func
	);
}

void cat::BenchMarker::display(bool convert_to_seconds)
{
	using LogSection = std::pair<std::string_view, std::vector<millis_t>>;

	const char* timescale = convert_to_seconds ? "s" : "ms";

	LOG_TEXT("----------------\n");
	LOG_TEXT("catacomb benchmarks:\n");
	LOG_TEXT("----------------\n");

	for(LogSection section : s_library)
	{
		const std::string_view& label = section.first;
		const std::vector<millis_t>& data = section.second;
		
		millis_t mean = 0;
		millis_t min = data[0];
		millis_t max = data[0];

		for(millis_t entry : data)
		{
			mean += entry;

			if(entry < min) min = entry;
			if(entry > max) max = entry;
		}

		mean /= data.size();

		f64 variance = _variance(data);
		f64 stddev = sqrt(variance);

		if(convert_to_seconds)
		{
			mean /= 1000;
			min /= 1000;
			max /= 1000;
		}

		LOG_TEXTF(
			"%s benchmarks:\n\tentries: %lu\n\tmean: %lu %s\n\tmin: %lu %s\n\tmax: %lu %s\n\tstddev: %.2f\n",
			label.data(),
			data.size(),
			mean, timescale,
			min, timescale,
			max, timescale,
			stddev
		)
	}
}

#endif
