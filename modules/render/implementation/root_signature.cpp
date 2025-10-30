module;

#include <cassert>
#include <cstdint>

#include <d3d12.h>

#include <wrl.h>

module render;

import :root_signature;
import :device;
import gg.render.helpers;

namespace gg
{

uint32_t RootSignature::getDescriptorTableBitMask(D3D12_DESCRIPTOR_HEAP_TYPE descriptor_heap_type)
{
    uint32_t descriptor_table_bit_mask = 0;
    switch(descriptor_heap_type)
    {
        case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
        {
            descriptor_table_bit_mask = this->descriptor_table_bit_mask;
            break;
        }
        case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:
        {
            descriptor_table_bit_mask = this->sampler_table_bit_mask;
            break;
        }
    }

    return descriptor_table_bit_mask;
}

uint32_t RootSignature::getNumDescriptors(uint32_t root_index) const
{
    assert(root_index < 32);
    return this->num_descriptors_per_table[root_index];
}

RootSignature::RootSignature(Device& device, const D3D12_ROOT_SIGNATURE_DESC1& desc)
    : device(device)
    , root_signature_desc{}
    , num_descriptors_per_table{0}
    , sampler_table_bit_mask{0}
    , descriptor_table_bot_mask{0}
{
    this->setRootSignatureDesc(desc);
}

RootSignature::~RootSignature()
{
    this->destroy();
}

void RootSignature::destroy()
{
    for(UINT i = 0; this->root_signature_desc.NumParameters; ++i)
    {
        const D3D12_ROOT_PARAMETER1& root_parameter = this->root_signature_desc.pParameters[i];
        if(root_parameter.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
        {
            delete[] root_parameter.DescriptorTable.pDescriptorRanges;
        }

        delete[] this->root_signature_desc.pParameters;
        this->root_signature_desc.pParameters = nullptr;
        this->root_signature_desc.NumParameters = 0;

        delete[] this->root_signature_desc.pStaticSamplers;
        this->root_signature_desc.pStaticSamplers   = nullptr;
        this->root_signature_desc.NumStaticSamplers = 0;

        memset(this->num_descriptors_per_table, 0, sizeof(this->num_descriptors_per_table));
    }
}

void RootSignature::setRootSignatureDesc(const D3D12_ROOT_SIGNATURE_DESC1& desc)
{
    this->destroy();

    UINT                   num_parameters = this->root_signature_desc.NumParameters;
    D3D12_ROOT_PARAMETER1* parameters   = num_parameters > 0 ? new D3D12_ROOT_PARAMETER1[num_parameters] : nullptr;

    for ( UINT i = 0; i < num_parameters; ++i )
    {
        const D3D12_ROOT_PARAMETER1& root_parameter = this->root_signature_desc.pParameters[i];
        parameters[i]                               = root_parameter;

        if ( root_parameter.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE )
        {
            UINT                     numDescriptorRanges = rootParameter.DescriptorTable.NumDescriptorRanges;
            D3D12_DESCRIPTOR_RANGE1* pDescriptorRanges =
                numDescriptorRanges > 0 ? new D3D12_DESCRIPTOR_RANGE1[numDescriptorRanges] : nullptr;

            memcpy( pDescriptorRanges, rootParameter.DescriptorTable.pDescriptorRanges,
                    sizeof( D3D12_DESCRIPTOR_RANGE1 ) * numDescriptorRanges );

            pParameters[i].DescriptorTable.NumDescriptorRanges = numDescriptorRanges;
            pParameters[i].DescriptorTable.pDescriptorRanges   = pDescriptorRanges;

            // Set the bit mask depending on the type of descriptor table.
            if ( numDescriptorRanges > 0 )
            {
                switch ( pDescriptorRanges[0].RangeType )
                {
                case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
                case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
                case D3D12_DESCRIPTOR_RANGE_TYPE_UAV:
                    m_DescriptorTableBitMask |= ( 1 << i );
                    break;
                case D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER:
                    m_SamplerTableBitMask |= ( 1 << i );
                    break;
                }
            }

            for ( UINT j = 0; j < numDescriptorRanges; ++j )
            { m_NumDescriptorsPerTable[i] += pDescriptorRanges[j].NumDescriptors; }
        }
    }

    m_RootSignatureDesc.NumParameters = numParameters;
    m_RootSignatureDesc.pParameters   = pParameters;

    UINT                       numStaticSamplers = rootSignatureDesc.NumStaticSamplers;
    D3D12_STATIC_SAMPLER_DESC* pStaticSamplers =
        numStaticSamplers > 0 ? new D3D12_STATIC_SAMPLER_DESC[numStaticSamplers] : nullptr;

    if ( pStaticSamplers )
    {
        memcpy( pStaticSamplers, rootSignatureDesc.pStaticSamplers,
                sizeof( D3D12_STATIC_SAMPLER_DESC ) * numStaticSamplers );
    }

    m_RootSignatureDesc.NumStaticSamplers = numStaticSamplers;
    m_RootSignatureDesc.pStaticSamplers   = pStaticSamplers;

    D3D12_ROOT_SIGNATURE_FLAGS flags = rootSignatureDesc.Flags;
    m_RootSignatureDesc.Flags        = flags;

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC versionRootSignatureDesc;
    versionRootSignatureDesc.Init_1_1( numParameters, pParameters, numStaticSamplers, pStaticSamplers, flags );

    D3D_ROOT_SIGNATURE_VERSION highestVersion = m_Device.GetHighestRootSignatureVersion();

    Microsoft::WRL::ComPtr<ID3DBlob> rootSignatureBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
    ThrowIfFailed( D3DX12SerializeVersionedRootSignature( &versionRootSignatureDesc, highestVersion,
                                                          &rootSignatureBlob, &errorBlob ) );

    auto d3d12Device = m_Device.GetD3D12Device();

    ThrowIfFailed( d3d12Device->CreateRootSignature( 0, rootSignatureBlob->GetBufferPointer(),
                                                     rootSignatureBlob->GetBufferSize(),
                                                     IID_PPV_ARGS( &m_RootSignature ) ) );
}

} // namespace gg;
