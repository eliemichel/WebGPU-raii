WebGPU RAII Wrapper
===================

This is a simple [RAII](https://en.cppreference.com/w/cpp/language/raii) wrapper around the objects of [the WebGPU native API](https://github.com/webgpu-native/webgpu-headers) that automatically releases them when they get out of scope.

A step by step introduction about its design is available in [the RAII chapter](https://eliemichel.github.io/LearnWebGPU/advanced-techniques/raii.html) of the LearnWebGPU guide.

**⚠️Disclaimer** *This is an early release that I don't find entierly satisfying, see the Limitations section below.*

Basic Usage
-----------

First of all, this assumes that you are already using my zero-overhead [`webgpu.hpp`](https://github.com/eliemichel/WebGPU-Cpp) wrapper to give a more C++ taste to the original C API.

Then simply add the file [`webgpu-raii.hpp`](webgpu-raii.hpp) to your source tree and include it.

For each object of the WebGPU API like `wgpu::Device` (a.k.a. `WGPUDevice`), the wrapper defines a `wgpu::raii::Device` object that behaves similarly except that it **can only be moved** (no copy semantics) and automatically calls `release()` when getting out of scope.

An RAII object can be created from a non RAII one by moving the non-RAII object. The latter must no longer be used once moved.

The RAII object can be dereferenced into the non RAII one as if it was a pointer, i.e., either usign the `*` operator, or through the `->` syntax.

```C++
#include "webgpu-raii.hpp"

using namespace wgpu;

// Without RAII
{
	Queue queue = device.getQueue();

	queue.submit(/* [...] */);

	queue.release();
}

// With RAII
{
	raii::Queue queue = device.getQueue();

	queue->submit(/* [...] */);
	//   ^^ Note that we use the arrow operator here to access the submit
	//      method of the underlying object wgpu::Queue.

	// No need to release, it is done automatically when getting out of scope.
}

// Convertion from non-RAII object
{
	Queue queue = device.getQueue();
	raii::Queue raiiQueue = std::move(queue);
	//                      ^^^^^^^^^ Move to make RAII the only owner of this

	raiiQueue->submit(/* [...] */);

	// No need to release anything
}
```

**☝️Important** *When the object that gets out of scope has a `destroy()` method (e.g., textures and buffers), this destroy method also gets invoked before releasing.*

Limitations
-----------

In its current state, this wrapper puzzles me for various reasons:

 1. It **does not take benefit from** the possibility to `reference()` objects to increase their internal reference counter. The main reason for this choice is that there is no way to query the reference counter's value to be able to call `destroy()` only before the ultimate call to `release()`. In the end, we are forced to maintain our own counter anyways, for instance by using a `std::unique_ptr<wgpu::raii::Foo>`. (Also, I like move-only semantics in general.)

 2. I'm not entirely sure whether the ownership model is very idiomatic. If somebody who's experience with RAII would pass by, I'm interested about feedback here!
