#include "opencl.hpp"
#include <cstdlib>
/////// OpenCL lesson #2 ////////////
// 1. Project Structure
// 2. Queuing func params (buffers) - where and how
// 3. Passing a struct
// 4. Atomic operations
// 5. Using atomic operations for sync.
/////////////////////////////////////

extern "C" {
#include "../Kernels/source.h"
}


void cpu_calc_hist(uint n, HistogramParams* H, float* arr, int* sum, int* hist) {

	float value = arr[n];

	float binRange = (H->maxHistValue - H->minHistValue) / (float)H->numBins;
	int bin = (int)((value - H->minHistValue) / binRange);
	if (bin < 0)
		bin = 0;
	if (bin > (H->numBins - 1))
		bin = H->numBins - 1;

	int prev_value = hist[bin]++;
	if (n == 0) {
		printf("\nbin Range=%f\n", binRange);
	}

	sum += (int)(1000 * value);

	printf("K#%d:value %f --> hist[%d]=%d \n", n, value, bin, prev_value + 1);

}


int main() {
	Device device(select_device_with_most_flops(), get_opencl_c_code()); // compile OpenCL C code for the fastest available device

	const uint N = 1024u; // size of vectors


	HistogramParams params;
	params.numBins = 10;
	params.minHistValue = 0.5;
	params.maxHistValue = 2.1;

	Memory<char> histParams(device, sizeof(HistogramParams));
	memcpy(histParams(), &params, sizeof(HistogramParams));


	Memory<float> heights(device, N);
	
	Memory<int> sum(device, 1);
	sum[0] = 0;

	Memory<int> hist(device, params.numBins);

	Kernel my_kernel(device, N, "calc_hist", histParams, heights, sum, hist); // kernel that runs on the device

	for (uint n = 0u; n < N; n++) {
		float randomHeight = 3 * static_cast<float>(rand()) / RAND_MAX;
		heights[n] = randomHeight;
	}
	for (int i = 0; i < params.numBins; i++)
	{
		hist[i] = 0;
	}

	histParams.write_to_device(); // copy data from host memory to device memory
	heights.write_to_device();
	sum.write_to_device();
	hist.write_to_device();
	my_kernel.run(); 

	hist.read_from_device();
	sum.read_from_device(); 


	for (int i = 0; i < params.numBins; i++)
	{
		printf("%d, ", hist[i]);
	}
	printf("\n sum = %f \n", (float)sum[0]/1000.f);



	//for (int n = 0; n < N; n++)
	//{
	//	cpu_calc_hist(n, &params, heights(), sum(), hist());
	//}

	return 0;
}