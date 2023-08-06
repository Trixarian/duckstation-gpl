// SPDX-FileCopyrightText: 2019-2022 Connor McLaughlin <stenzek@gmail.com>
// SPDX-License-Identifier: (GPL-3.0 OR CC-BY-NC-ND-4.0)

#pragma once

#if 0
#include "common/dimensional_array.h"
#include "gpu/d3d12/staging_texture.h"
#include "gpu/d3d12/stream_buffer.h"
#include "gpu/d3d12/texture.h"
#include "gpu_hw.h"
#include "texture_replacements.h"
#include <array>
#include <memory>
#include <tuple>

class GPU_HW_D3D12 final : public GPU_HW
{
public:
  template<typename T>
  using ComPtr = Microsoft::WRL::ComPtr<T>;

  GPU_HW_D3D12();
  ~GPU_HW_D3D12() override;

  bool Initialize() override;
  void Reset(bool clear_vram) override;

  void RestoreGraphicsAPIState() override;
  void UpdateSettings() override;

protected:
  void ClearDisplay() override;
  void UpdateDisplay() override;
  void ReadVRAM(u32 x, u32 y, u32 width, u32 height) override;
  void FillVRAM(u32 x, u32 y, u32 width, u32 height, u32 color) override;
  void UpdateVRAM(u32 x, u32 y, u32 width, u32 height, const void* data, bool set_mask, bool check_mask) override;
  void CopyVRAM(u32 src_x, u32 src_y, u32 dst_x, u32 dst_y, u32 width, u32 height) override;

private:
  enum : u32
  {
    MAX_PUSH_CONSTANTS_SIZE = 64,
    TEXTURE_REPLACEMENT_BUFFER_SIZE = 64 * 1024 * 1024,
  };
  void SetCapabilities();
  void DestroyResources();

  bool CreateRootSignatures();
  bool CreateSamplers();

  bool CreateBuffers();
  void ClearFramebuffer();
  void DestroyBuffers();

  bool CreateVertexBuffer();
  bool CreateUniformBuffer();
  bool CreateTextureBuffer();

  bool CompilePipelines();
  void DestroyPipelines();

  bool CreateTextureReplacementStreamBuffer();
  bool BlitVRAMReplacementTexture(const TextureReplacementTexture* tex, u32 dst_x, u32 dst_y, u32 width, u32 height);

  ComPtr<ID3D12RootSignature> m_batch_root_signature;
  ComPtr<ID3D12RootSignature> m_single_sampler_root_signature;

  D3D12::Texture m_vram_texture;
  D3D12::Texture m_vram_depth_texture;
  D3D12::Texture m_vram_read_texture;
  D3D12::Texture m_vram_readback_texture;
  D3D12::StagingTexture m_vram_readback_staging_texture;
  D3D12::Texture m_display_texture;

  D3D12::DescriptorHandle m_point_sampler;
  D3D12::DescriptorHandle m_linear_sampler;

  D3D12::StreamBuffer m_vertex_stream_buffer;
  D3D12::StreamBuffer m_uniform_stream_buffer;
  D3D12::StreamBuffer m_texture_stream_buffer;
  D3D12::DescriptorHandle m_texture_stream_buffer_srv;

  u32 m_current_uniform_buffer_offset = 0;

  // [depth_test][render_mode][texture_mode][transparency_mode][dithering][interlacing]
  DimensionalArray<ComPtr<ID3D12PipelineState>, 2, 2, 5, 9, 4, 2> m_batch_pipelines;

  // [wrapped][interlaced]
  DimensionalArray<ComPtr<ID3D12PipelineState>, 2, 2> m_vram_fill_pipelines;

  // [depth_test]
  std::array<ComPtr<ID3D12PipelineState>, 2> m_vram_write_pipelines;
  std::array<ComPtr<ID3D12PipelineState>, 2> m_vram_copy_pipelines;

  ComPtr<ID3D12PipelineState> m_vram_readback_pipeline;
  ComPtr<ID3D12PipelineState> m_vram_update_depth_pipeline;

  // [depth_24][interlace_mode]
  DimensionalArray<ComPtr<ID3D12PipelineState>, 3, 2> m_display_pipelines;

  ComPtr<ID3D12PipelineState> m_copy_pipeline;
  D3D12::Texture m_vram_write_replacement_texture;
  D3D12::StreamBuffer m_texture_replacment_stream_buffer;
};

#endif