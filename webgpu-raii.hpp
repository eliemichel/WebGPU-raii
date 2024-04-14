/**
 * This is a RAII wrapper for WebGPU native API.
 * 
 * This file is part of the "Learn WebGPU for C++" book.
 *   https://eliemichel.github.io/LearnWebGPU
 * 
 * MIT License
 * Copyright (c) 2022-2024 Elie Michel
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <webgpu/webgpu.hpp>

#include <type_traits>

namespace wgpu {
namespace raii {

// Utility template functions to detect the presence of a destroy() method
template <typename T, typename = void>
struct has_destroy : std::false_type { };
template <typename T>
struct has_destroy <T, decltype(std::declval<T>().destroy())> : std::true_type { };
// Sanity check
static_assert(has_destroy<wgpu::Texture>::value);
static_assert(!has_destroy<wgpu::Queue>::value);

/**
 * RAII wrapper around a raw WebGPU type.
 * Use pointer-like dereferencing to access methods from the wrapped type.
 */
template <typename Raw>
class Wrapper {
public:
	Wrapper()
		: m_raw(nullptr)
	{}

	Wrapper(Raw&& raw)
		: m_raw(raw)
	{}

	// We define a destructor...
	~Wrapper() {
		Destruct();
	}

	// Delete copy semantics
	// TODO: Use reference() instead now that it is supported everywhere
	Wrapper& operator=(const Wrapper& other) = delete;
	Wrapper(const Wrapper& other) = delete;

	// Move semantics
	Wrapper& operator=(Wrapper&& other) {
		Destruct();
		assert(m_raw == nullptr);
		m_raw = other.m_raw;
		other.m_raw = nullptr;
		return *this;
	}

	Wrapper(Wrapper&& other)
		: m_raw(other.m_raw)
	{
		other.m_raw = nullptr;
	}

	operator bool() const { return m_raw; }
	const Raw& operator*() const { return m_raw; }
	Raw& operator*() { return m_raw; }
	const Raw* operator->() const { return &m_raw; }
	Raw* operator->() { return &m_raw; }

private:
	void Destruct() {
		if (!m_raw) return;

		// Call destroy() if it does exist
		if constexpr (has_destroy<Raw>::value) {
			m_raw.destroy();
		}

		// Call release
		m_raw.release();

		m_raw = nullptr;
	}

private:
	// Raw resources that is wrapped by the RAII class
	Raw m_raw;
};

using Instance = wgpu::raii::Wrapper<wgpu::Instance>;
using Surface = wgpu::raii::Wrapper<wgpu::Surface>;
using Adapter = wgpu::raii::Wrapper<wgpu::Adapter>;
using Device = wgpu::raii::Wrapper<wgpu::Device>;
using Queue = wgpu::raii::Wrapper<wgpu::Queue>;
using SwapChain = wgpu::raii::Wrapper<wgpu::SwapChain>;
using RenderPassEncoder = wgpu::raii::Wrapper<wgpu::RenderPassEncoder>;
using Texture = wgpu::raii::Wrapper<wgpu::Texture>;
using TextureView = wgpu::raii::Wrapper<wgpu::TextureView>;
using Buffer = wgpu::raii::Wrapper<wgpu::Buffer>;
using CommandEncoder = wgpu::raii::Wrapper<wgpu::CommandEncoder>;
using RenderPassEncoder = wgpu::raii::Wrapper<wgpu::RenderPassEncoder>;
using ComputePassEncoder = wgpu::raii::Wrapper<wgpu::ComputePassEncoder>;
using CommandBuffer = wgpu::raii::Wrapper<wgpu::CommandBuffer>;
using BindGroup = wgpu::raii::Wrapper<wgpu::BindGroup>;
using BindGroupLayout = wgpu::raii::Wrapper<wgpu::BindGroupLayout>;
using RenderPipeline = wgpu::raii::Wrapper<wgpu::RenderPipeline>;
using PipelineLayout = wgpu::raii::Wrapper<wgpu::PipelineLayout>;
using ShaderModule = wgpu::raii::Wrapper<wgpu::ShaderModule>;
using ComputePipeline = wgpu::raii::Wrapper<wgpu::ComputePipeline>;
using Sampler = wgpu::raii::Wrapper<wgpu::Sampler>;
using QuerySet = wgpu::raii::Wrapper<wgpu::QuerySet>;
// TODO: Add other types

} // namespace raii
} // namespace wgpu
