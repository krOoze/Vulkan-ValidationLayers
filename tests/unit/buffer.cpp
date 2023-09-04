/*
 * Copyright (c) 2015-2023 The Khronos Group Inc.
 * Copyright (c) 2015-2023 Valve Corporation
 * Copyright (c) 2015-2023 LunarG, Inc.
 * Copyright (c) 2015-2023 Google, Inc.
 * Modifications Copyright (C) 2020 Advanced Micro Devices, Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 */

#include "utils/cast_utils.h"
#include "generated/enum_flag_bits.h"
#include "../framework/layer_validation_tests.h"
#include "../framework/pipeline_helper.h"
#include "utils/vk_layer_utils.h"

TEST_F(NegativeBuffer, Extents) {
    TEST_DESCRIPTION("Perform copies across a buffer, provoking out-of-range errors.");

    AddOptionalExtensions(VK_KHR_COPY_COMMANDS_2_EXTENSION_NAME);
    ASSERT_NO_FATAL_FAILURE(Init());
    const bool copy_commands2 = IsExtensionsEnabled(VK_KHR_COPY_COMMANDS_2_EXTENSION_NAME);

    VkBufferObj buffer_one(*m_device, 2048, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    VkBufferObj buffer_two(*m_device, 2048, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

    VkBufferCopy copy_info = {4096, 256, 256};

    m_commandBuffer->begin();

    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdCopyBuffer-srcOffset-00113");
    vk::CmdCopyBuffer(m_commandBuffer->handle(), buffer_one.handle(), buffer_two.handle(), 1, &copy_info);
    m_errorMonitor->VerifyFound();

    // equivalent test using KHR_copy_commands2
    if (copy_commands2) {
        const VkBufferCopy2KHR copy_info2 = {VK_STRUCTURE_TYPE_BUFFER_COPY_2_KHR, NULL, copy_info.srcOffset, copy_info.dstOffset,
                                             copy_info.size};
        const VkCopyBufferInfo2KHR copy_buffer_info2 = {
            VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2_KHR, NULL, buffer_one.handle(), buffer_two.handle(), 1, &copy_info2};
        m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-VkCopyBufferInfo2-srcOffset-00113");
        vk::CmdCopyBuffer2KHR(m_commandBuffer->handle(), &copy_buffer_info2);
        m_errorMonitor->VerifyFound();
    }

    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdCopyBuffer-dstOffset-00114");
    copy_info = {256, 4096, 256};
    vk::CmdCopyBuffer(m_commandBuffer->handle(), buffer_one.handle(), buffer_two.handle(), 1, &copy_info);
    m_errorMonitor->VerifyFound();

    // equivalent test using KHR_copy_commands2
    if (copy_commands2) {
        const VkBufferCopy2KHR copy_info2 = {VK_STRUCTURE_TYPE_BUFFER_COPY_2_KHR, NULL, copy_info.srcOffset, copy_info.dstOffset,
                                             copy_info.size};
        const VkCopyBufferInfo2KHR copy_buffer_info2 = {
            VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2_KHR, NULL, buffer_one.handle(), buffer_two.handle(), 1, &copy_info2};
        m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-VkCopyBufferInfo2-dstOffset-00114");
        vk::CmdCopyBuffer2KHR(m_commandBuffer->handle(), &copy_buffer_info2);
        m_errorMonitor->VerifyFound();
    }

    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdCopyBuffer-size-00115");
    copy_info = {1024, 256, 1280};
    vk::CmdCopyBuffer(m_commandBuffer->handle(), buffer_one.handle(), buffer_two.handle(), 1, &copy_info);
    m_errorMonitor->VerifyFound();

    // equivalent test using KHR_copy_commands2
    if (copy_commands2) {
        const VkBufferCopy2KHR copy_info2 = {VK_STRUCTURE_TYPE_BUFFER_COPY_2_KHR, NULL, copy_info.srcOffset, copy_info.dstOffset,
                                             copy_info.size};
        const VkCopyBufferInfo2KHR copy_buffer_info2 = {
            VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2_KHR, NULL, buffer_one.handle(), buffer_two.handle(), 1, &copy_info2};
        m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-VkCopyBufferInfo2-size-00115");
        vk::CmdCopyBuffer2KHR(m_commandBuffer->handle(), &copy_buffer_info2);
        m_errorMonitor->VerifyFound();
    }

    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdCopyBuffer-size-00116");
    copy_info = {256, 1024, 1280};
    vk::CmdCopyBuffer(m_commandBuffer->handle(), buffer_one.handle(), buffer_two.handle(), 1, &copy_info);
    m_errorMonitor->VerifyFound();

    // equivalent test using KHR_copy_commands2
    if (copy_commands2) {
        const VkBufferCopy2KHR copy_info2 = {VK_STRUCTURE_TYPE_BUFFER_COPY_2_KHR, NULL, copy_info.srcOffset, copy_info.dstOffset,
                                             copy_info.size};
        const VkCopyBufferInfo2KHR copy_buffer_info2 = {
            VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2_KHR, NULL, buffer_one.handle(), buffer_two.handle(), 1, &copy_info2};
        m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-VkCopyBufferInfo2-size-00116");
        vk::CmdCopyBuffer2KHR(m_commandBuffer->handle(), &copy_buffer_info2);
        m_errorMonitor->VerifyFound();
    }

    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdCopyBuffer-pRegions-00117");
    copy_info = {256, 512, 512};
    vk::CmdCopyBuffer(m_commandBuffer->handle(), buffer_two.handle(), buffer_two.handle(), 1, &copy_info);
    m_errorMonitor->VerifyFound();

    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-VkBufferCopy-size-01988");
    copy_info = {256, 256, 0};
    vk::CmdCopyBuffer(m_commandBuffer->handle(), buffer_two.handle(), buffer_two.handle(), 1, &copy_info);
    m_errorMonitor->VerifyFound();

    m_commandBuffer->end();
}

TEST_F(NegativeBuffer, UpdateBufferAlignment) {
    TEST_DESCRIPTION("Check alignment parameters for vkCmdUpdateBuffer");
    uint32_t updateData[] = {1, 2, 3, 4, 5, 6, 7, 8};

    ASSERT_NO_FATAL_FAILURE(Init());
    VkBufferObj buffer(*m_device, 20, VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

    m_commandBuffer->begin();
    // Introduce failure by using dstOffset that is not multiple of 4
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, " is not a multiple of 4");
    m_commandBuffer->UpdateBuffer(buffer.handle(), 1, 4, updateData);
    m_errorMonitor->VerifyFound();

    // Introduce failure by using dataSize that is not multiple of 4
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, " is not a multiple of 4");
    m_commandBuffer->UpdateBuffer(buffer.handle(), 0, 6, updateData);
    m_errorMonitor->VerifyFound();

    // Introduce failure by using dataSize that is < 0
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "must be greater than zero and less than or equal to 65536");
    m_commandBuffer->UpdateBuffer(buffer.handle(), 0, (VkDeviceSize)-44, updateData);
    m_errorMonitor->VerifyFound();

    // Introduce failure by using dataSize that is > 65536
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "must be greater than zero and less than or equal to 65536");
    m_commandBuffer->UpdateBuffer(buffer.handle(), 0, (VkDeviceSize)80000, updateData);
    m_errorMonitor->VerifyFound();

    m_commandBuffer->end();
}

TEST_F(NegativeBuffer, FillBufferAlignmentAndSize) {
    TEST_DESCRIPTION("Check alignment and size parameters for vkCmdFillBuffer");

    ASSERT_NO_FATAL_FAILURE(Init());
    VkBufferObj buffer(*m_device, 20, VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    m_commandBuffer->begin();

    // Introduce failure by using dstOffset greater than bufferSize
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdFillBuffer-dstOffset-00024");
    m_commandBuffer->FillBuffer(buffer.handle(), 40, 4, 0x11111111);
    m_errorMonitor->VerifyFound();

    // Introduce failure by using size <= buffersize minus dstoffset
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdFillBuffer-size-00027");
    m_commandBuffer->FillBuffer(buffer.handle(), 16, 12, 0x11111111);
    m_errorMonitor->VerifyFound();

    // Introduce failure by using dstOffset that is not multiple of 4
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, " is not a multiple of 4");
    m_commandBuffer->FillBuffer(buffer.handle(), 1, 4, 0x11111111);
    m_errorMonitor->VerifyFound();

    // Introduce failure by using size that is not multiple of 4
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, " is not a multiple of 4");
    m_commandBuffer->FillBuffer(buffer.handle(), 0, 6, 0x11111111);
    m_errorMonitor->VerifyFound();

    // Introduce failure by using size that is zero
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "must be greater than zero");
    m_commandBuffer->FillBuffer(buffer.handle(), 0, 0, 0x11111111);
    m_errorMonitor->VerifyFound();

    m_commandBuffer->end();
}

TEST_F(NegativeBuffer, BufferViewObject) {
    // Create a single TEXEL_BUFFER descriptor and send it an invalid bufferView
    // First, cause the bufferView to be invalid due to underlying buffer being destroyed
    // Then destroy view itself and verify that same error is hit
    VkResult err;

    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-VkWriteDescriptorSet-descriptorType-02994");
    ASSERT_NO_FATAL_FAILURE(Init());

    OneOffDescriptorSet descriptor_set(m_device, {
                                                     {0, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1, VK_SHADER_STAGE_ALL, nullptr},
                                                 });
    VkBufferView view;
    {
        // Create a valid bufferView to start with
        uint32_t queue_family_index = 0;
        VkBufferCreateInfo buffer_create_info = LvlInitStruct<VkBufferCreateInfo>();
        buffer_create_info.size = 1024;
        buffer_create_info.usage = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
        buffer_create_info.queueFamilyIndexCount = 1;
        buffer_create_info.pQueueFamilyIndices = &queue_family_index;
        VkBufferObj buffer(*m_device, buffer_create_info);

        VkBufferViewCreateInfo bvci = LvlInitStruct<VkBufferViewCreateInfo>();
        bvci.buffer = buffer.handle();
        bvci.format = VK_FORMAT_R32_SFLOAT;
        bvci.range = VK_WHOLE_SIZE;

        err = vk::CreateBufferView(m_device->device(), &bvci, NULL, &view);
        ASSERT_VK_SUCCESS(err);
    }
    // First Destroy buffer underlying view which should hit error in CV

    VkWriteDescriptorSet descriptor_write = LvlInitStruct<VkWriteDescriptorSet>();
    descriptor_write.dstSet = descriptor_set.set_;
    descriptor_write.dstBinding = 0;
    descriptor_write.descriptorCount = 1;
    descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
    descriptor_write.pTexelBufferView = &view;

    vk::UpdateDescriptorSets(m_device->device(), 1, &descriptor_write, 0, NULL);
    m_errorMonitor->VerifyFound();

    // Now destroy view itself and verify same error, which is hit in PV this time
    vk::DestroyBufferView(m_device->device(), view, NULL);
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-VkWriteDescriptorSet-descriptorType-02994");
    vk::UpdateDescriptorSets(m_device->device(), 1, &descriptor_write, 0, NULL);
    m_errorMonitor->VerifyFound();
}

TEST_F(NegativeBuffer, CreateBufferViewNoMemoryBoundToBuffer) {
    TEST_DESCRIPTION("Attempt to create a buffer view with a buffer that has no memory bound to it.");

    VkResult err;
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit,
                                         " used with no memory bound. Memory should be bound by calling vkBindBufferMemory().");

    ASSERT_NO_FATAL_FAILURE(Init());

    // Create a buffer with no bound memory and then attempt to create
    // a buffer view.
    VkBufferCreateInfo buff_ci = LvlInitStruct<VkBufferCreateInfo>();
    buff_ci.usage = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
    buff_ci.size = 256;
    buff_ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VkBuffer buffer;
    err = vk::CreateBuffer(m_device->device(), &buff_ci, NULL, &buffer);
    ASSERT_VK_SUCCESS(err);

    VkBufferViewCreateInfo buff_view_ci = LvlInitStruct<VkBufferViewCreateInfo>();
    buff_view_ci.buffer = buffer;
    buff_view_ci.format = VK_FORMAT_R8_UNORM;
    buff_view_ci.range = VK_WHOLE_SIZE;
    vk_testing::BufferView buffer_view(*m_device, buff_view_ci);
    m_errorMonitor->VerifyFound();
    vk::DestroyBuffer(m_device->device(), buffer, NULL);
}

TEST_F(NegativeBuffer, BufferViewCreateInfoEntries) {
    TEST_DESCRIPTION("Attempt to create a buffer view with invalid create info.");

    // Attempt to enable texel buffer alignmnet extension
    AddOptionalExtensions(VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME);
    ASSERT_NO_FATAL_FAILURE(Init());

    const VkPhysicalDeviceLimits &dev_limits = m_device->props.limits;
    const VkDeviceSize minTexelBufferOffsetAlignment = dev_limits.minTexelBufferOffsetAlignment;
    if (minTexelBufferOffsetAlignment == 1) {
        GTEST_SKIP() << "Test requires minTexelOffsetAlignment to not be equal to 1";
    }

    const VkFormat format_with_uniform_texel_support = VK_FORMAT_R8G8B8A8_UNORM;
    const VkFormat format_without_texel_support = VK_FORMAT_R8G8B8_UNORM;
    VkFormatProperties format_properties;
    vk::GetPhysicalDeviceFormatProperties(gpu(), format_with_uniform_texel_support, &format_properties);
    if (!(format_properties.bufferFeatures & VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT)) {
        GTEST_SKIP() << "Test requires support for VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT";
    }

    // Create a test buffer--buffer must have been created using VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT or
    // VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT, so use a different usage value instead to cause an error
    const VkDeviceSize resource_size = 1024;
    const VkBufferCreateInfo bad_buffer_info = VkBufferObj::create_info(resource_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    VkBufferObj bad_buffer(*m_device, bad_buffer_info, (VkMemoryPropertyFlags)VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    // Create a test buffer view
    VkBufferViewCreateInfo buff_view_ci = LvlInitStruct<VkBufferViewCreateInfo>();
    buff_view_ci.buffer = bad_buffer.handle();
    buff_view_ci.format = format_with_uniform_texel_support;
    buff_view_ci.range = VK_WHOLE_SIZE;
    CreateBufferViewTest(*this, &buff_view_ci, {"VUID-VkBufferViewCreateInfo-buffer-00932"});

    // Create a better test buffer
    const VkBufferCreateInfo buffer_info = VkBufferObj::create_info(resource_size, VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT);
    VkBufferObj buffer(*m_device, buffer_info, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    // Offset must be less than the size of the buffer, so set it equal to the buffer size to cause an error
    buff_view_ci.buffer = buffer.handle();
    buff_view_ci.offset = buffer.create_info().size;
    CreateBufferViewTest(*this, &buff_view_ci, {"VUID-VkBufferViewCreateInfo-offset-00925"});

    // Offset must be a multiple of VkPhysicalDeviceLimits::minTexelBufferOffsetAlignment so add 1 to ensure it is not
    buff_view_ci.offset = minTexelBufferOffsetAlignment + 1;
    CreateBufferViewTest(*this, &buff_view_ci, {"VUID-VkBufferViewCreateInfo-offset-02749"});

    // Set offset to acceptable value for range tests
    buff_view_ci.offset = minTexelBufferOffsetAlignment;
    // Setting range equal to 0 will cause an error to occur
    buff_view_ci.range = 0;
    CreateBufferViewTest(*this, &buff_view_ci, {"VUID-VkBufferViewCreateInfo-range-00928"});

    uint32_t format_size = FormatElementSize(buff_view_ci.format);
    // Range must be a multiple of the element size of format, so add one to ensure it is not
    buff_view_ci.range = format_size + 1;
    CreateBufferViewTest(*this, &buff_view_ci, {"VUID-VkBufferViewCreateInfo-range-00929"});

    // Twice the element size of format multiplied by VkPhysicalDeviceLimits::maxTexelBufferElements guarantees range divided by the
    // element size is greater than maxTexelBufferElements, causing failure
    buff_view_ci.range = 2 * static_cast<VkDeviceSize>(format_size) * static_cast<VkDeviceSize>(dev_limits.maxTexelBufferElements);
    CreateBufferViewTest(*this, &buff_view_ci,
                         {"VUID-VkBufferViewCreateInfo-range-00930", "VUID-VkBufferViewCreateInfo-offset-00931"});

    // Create a new test buffer that is larger than VkPhysicalDeviceLimits::maxTexelBufferElements
    // The spec min max is just 64K, but some implementations support a much larger value than that.
    // Skip the test if the limit is very large to not allocate excessive amounts of memory.
    if (dev_limits.maxTexelBufferElements > 64 * 1024 * 1024) {
        printf("Test skipped if maxTexelBufferElements is very large. \n");
    } else {
        const VkDeviceSize large_resource_size =
            2 * static_cast<VkDeviceSize>(format_size) * static_cast<VkDeviceSize>(dev_limits.maxTexelBufferElements);
        const VkBufferCreateInfo large_buffer_info =
            VkBufferObj::create_info(large_resource_size, VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT);
        VkBufferObj large_buffer(*m_device, large_buffer_info, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // Offset must be less than the size of the buffer, so set it equal to the buffer size to cause an error
        buff_view_ci.buffer = large_buffer.handle();
        buff_view_ci.range = VK_WHOLE_SIZE;

        // For VK_WHOLE_SIZE, the buffer size - offset must be less than VkPhysicalDeviceLimits::maxTexelBufferElements
        CreateBufferViewTest(*this, &buff_view_ci, {"VUID-VkBufferViewCreateInfo-range-04059"});
    }

    vk::GetPhysicalDeviceFormatProperties(gpu(), format_without_texel_support, &format_properties);
    if ((format_properties.bufferFeatures & VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT) ||
        (format_properties.bufferFeatures & VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT)) {
        GTEST_SKIP()
            << "Test requires no support for VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT nor VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT";
    }

    // Set range to acceptable value for buffer tests
    buff_view_ci.buffer = buffer.handle();
    buff_view_ci.format = format_without_texel_support;
    buff_view_ci.range = VK_WHOLE_SIZE;

    // `buffer` was created using VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT so we can use that for the first buffer test
    CreateBufferViewTest(*this, &buff_view_ci, {"VUID-VkBufferViewCreateInfo-buffer-00933"});

    // Create a new buffer using VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT
    const VkBufferCreateInfo storage_buffer_info =
        VkBufferObj::create_info(resource_size, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT);
    VkBufferObj storage_buffer(*m_device, storage_buffer_info, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    buff_view_ci.buffer = storage_buffer.handle();
    CreateBufferViewTest(*this, &buff_view_ci, {"VUID-VkBufferViewCreateInfo-buffer-00934"});
}

TEST_F(NegativeBuffer, TexelBufferAlignmentIn12) {
    TEST_DESCRIPTION("texelBufferAlignment is not enabled by default in 1.2.");
    SetTargetApiVersion(VK_API_VERSION_1_2);
    ASSERT_NO_FATAL_FAILURE(Init());
    if (DeviceValidationVersion() >= VK_API_VERSION_1_3) {
        GTEST_SKIP() << "Vulkan version 1.2 or less is required";
    }

    const VkDeviceSize minTexelBufferOffsetAlignment = m_device->props.limits.minTexelBufferOffsetAlignment;
    if (minTexelBufferOffsetAlignment == 1) {
        GTEST_SKIP() << "Test requires minTexelOffsetAlignment to not be equal to 1";
    }

    VkFormatProperties format_properties;
    vk::GetPhysicalDeviceFormatProperties(gpu(), VK_FORMAT_R8G8B8A8_UNORM, &format_properties);
    if (!(format_properties.bufferFeatures & VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT)) {
        GTEST_SKIP() << "Test requires support for VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT";
    }

    const VkBufferCreateInfo buffer_info = VkBufferObj::create_info(1024, VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT);
    VkBufferObj buffer(*m_device, buffer_info, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    VkBufferViewCreateInfo buff_view_ci = LvlInitStruct<VkBufferViewCreateInfo>();
    buff_view_ci.format = VK_FORMAT_R8G8B8A8_UNORM;
    buff_view_ci.range = VK_WHOLE_SIZE;
    buff_view_ci.buffer = buffer.handle();
    buff_view_ci.offset = minTexelBufferOffsetAlignment + 1;
    CreateBufferViewTest(*this, &buff_view_ci, {"VUID-VkBufferViewCreateInfo-offset-02749"});
}

TEST_F(NegativeBuffer, TexelBufferAlignment) {
    TEST_DESCRIPTION("Test VK_EXT_texel_buffer_alignment.");
    AddRequiredExtensions(VK_EXT_TEXEL_BUFFER_ALIGNMENT_EXTENSION_NAME);
    ASSERT_NO_FATAL_FAILURE(InitFramework());
    if (!AreRequiredExtensionsEnabled()) {
        GTEST_SKIP() << RequiredExtensionsNotSupported() << " not supported";
    }

    auto texel_buffer_alignment_features = LvlInitStruct<VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT>();
    GetPhysicalDeviceFeatures2(texel_buffer_alignment_features);
    if (texel_buffer_alignment_features.texelBufferAlignment != VK_TRUE) {
        GTEST_SKIP() << "texelBufferAlignment feature not supported";
    }

    auto align_props = LvlInitStruct<VkPhysicalDeviceTexelBufferAlignmentPropertiesEXT>();
    GetPhysicalDeviceProperties2(align_props);

    ASSERT_NO_FATAL_FAILURE(InitState(nullptr, &texel_buffer_alignment_features));
    ASSERT_NO_FATAL_FAILURE(InitRenderTarget());

    const VkFormat format_with_uniform_texel_support = VK_FORMAT_R8G8B8A8_UNORM;

    const VkDeviceSize resource_size = 1024;
    VkBufferCreateInfo buffer_info = VkBufferObj::create_info(
        resource_size, VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT);
    VkBufferObj buffer(*m_device, buffer_info, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    // Create a test buffer view
    VkBufferViewCreateInfo buff_view_ci = LvlInitStruct<VkBufferViewCreateInfo>();
    buff_view_ci.buffer = buffer.handle();
    buff_view_ci.format = format_with_uniform_texel_support;
    buff_view_ci.range = VK_WHOLE_SIZE;

    buff_view_ci.offset = 1;
    std::vector<std::string> expectedErrors;
    if (buff_view_ci.offset < align_props.storageTexelBufferOffsetAlignmentBytes) {
        expectedErrors.push_back("VUID-VkBufferViewCreateInfo-buffer-02750");
    }
    if (buff_view_ci.offset < align_props.uniformTexelBufferOffsetAlignmentBytes) {
        expectedErrors.push_back("VUID-VkBufferViewCreateInfo-buffer-02751");
    }
    CreateBufferViewTest(*this, &buff_view_ci, expectedErrors);
    expectedErrors.clear();

    buff_view_ci.offset = 4;
    if (buff_view_ci.offset < align_props.storageTexelBufferOffsetAlignmentBytes &&
        !align_props.storageTexelBufferOffsetSingleTexelAlignment) {
        expectedErrors.push_back("VUID-VkBufferViewCreateInfo-buffer-02750");
    }
    if (buff_view_ci.offset < align_props.uniformTexelBufferOffsetAlignmentBytes &&
        !align_props.uniformTexelBufferOffsetSingleTexelAlignment) {
        expectedErrors.push_back("VUID-VkBufferViewCreateInfo-buffer-02751");
    }
    CreateBufferViewTest(*this, &buff_view_ci, expectedErrors);
    expectedErrors.clear();

    // Test a 3-component format
    VkFormatProperties format_properties;
    vk::GetPhysicalDeviceFormatProperties(gpu(), VK_FORMAT_R32G32B32_SFLOAT, &format_properties);
    if (format_properties.bufferFeatures & VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT) {
        buffer_info.usage = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
        VkBufferObj buffer2;
        buffer2.init(*m_device, buffer_info, (VkMemoryPropertyFlags)VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // Create a test buffer view
        buff_view_ci.buffer = buffer2.handle();

        buff_view_ci.format = VK_FORMAT_R32G32B32_SFLOAT;
        buff_view_ci.offset = 1;
        if (buff_view_ci.offset < align_props.uniformTexelBufferOffsetAlignmentBytes) {
            expectedErrors.push_back("VUID-VkBufferViewCreateInfo-buffer-02751");
        }
        CreateBufferViewTest(*this, &buff_view_ci, expectedErrors);
        expectedErrors.clear();

        buff_view_ci.offset = 4;
        if (buff_view_ci.offset < align_props.uniformTexelBufferOffsetAlignmentBytes &&
            !align_props.uniformTexelBufferOffsetSingleTexelAlignment) {
            expectedErrors.push_back("VUID-VkBufferViewCreateInfo-buffer-02751");
        }
        CreateBufferViewTest(*this, &buff_view_ci, expectedErrors);
        expectedErrors.clear();
    }
}

TEST_F(NegativeBuffer, FillBufferWithinRenderPass) {
    // Call CmdFillBuffer within an active renderpass
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdFillBuffer-renderpass");

    ASSERT_NO_FATAL_FAILURE(Init());
    ASSERT_NO_FATAL_FAILURE(InitRenderTarget());

    m_commandBuffer->begin();
    m_commandBuffer->BeginRenderPass(m_renderPassBeginInfo);

    VkMemoryPropertyFlags reqs = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    VkBufferObj dst_buffer(*m_device, 1024, VK_BUFFER_USAGE_TRANSFER_DST_BIT, reqs);
    m_commandBuffer->FillBuffer(dst_buffer.handle(), 0, 4, 0x11111111);

    m_errorMonitor->VerifyFound();

    m_commandBuffer->EndRenderPass();
    m_commandBuffer->end();
}

TEST_F(NegativeBuffer, UpdateBufferWithinRenderPass) {
    // Call CmdUpdateBuffer within an active renderpass
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdUpdateBuffer-renderpass");

    ASSERT_NO_FATAL_FAILURE(Init());
    ASSERT_NO_FATAL_FAILURE(InitRenderTarget());

    m_commandBuffer->begin();
    m_commandBuffer->BeginRenderPass(m_renderPassBeginInfo);

    VkBufferObj dst_buffer(*m_device, 1024, VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

    VkDeviceSize dstOffset = 0;
    uint32_t Data[] = {1, 2, 3, 4, 5, 6, 7, 8};
    VkDeviceSize dataSize = sizeof(Data) / sizeof(uint32_t);
    vk::CmdUpdateBuffer(m_commandBuffer->handle(), dst_buffer.handle(), dstOffset, dataSize, &Data);

    m_errorMonitor->VerifyFound();

    m_commandBuffer->EndRenderPass();
    m_commandBuffer->end();
}

TEST_F(NegativeBuffer, IdxBufferAlignmentError) {
    // Bind a BeginRenderPass within an active RenderPass
    ASSERT_NO_FATAL_FAILURE(Init());
    ASSERT_NO_FATAL_FAILURE(InitRenderTarget());

    uint32_t const indices[] = {0};
    auto buf_info = LvlInitStruct<VkBufferCreateInfo>();
    buf_info.size = 1024;
    buf_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    buf_info.queueFamilyIndexCount = 1;
    buf_info.pQueueFamilyIndices = indices;
    VkBufferObj buffer(*m_device, buf_info);

    m_commandBuffer->begin();
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdBindIndexBuffer-offset-08783");
    vk::CmdBindIndexBuffer(m_commandBuffer->handle(), buffer.handle(), 7, VK_INDEX_TYPE_UINT16);
    m_errorMonitor->VerifyFound();
}

TEST_F(NegativeBuffer, VertexBuffer) {
    TEST_DESCRIPTION(
        "Submit a command buffer using deleted vertex buffer, delete a buffer twice, use an invalid offset for each buffer type, "
        "and attempt to bind a null buffer");

    ASSERT_NO_FATAL_FAILURE(Init());
    ASSERT_NO_FATAL_FAILURE(InitViewport());
    ASSERT_NO_FATAL_FAILURE(InitRenderTarget());

    CreatePipelineHelper pipe(*this);
    pipe.InitState();
    pipe.CreateGraphicsPipeline();

    m_commandBuffer->begin();
    m_commandBuffer->BeginRenderPass(m_renderPassBeginInfo);
    vk::CmdBindPipeline(m_commandBuffer->handle(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipe.pipeline_);
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "CoreValidation-DrawState-InvalidCommandBuffer-VkBuffer");

    {
        VkDeviceSize offset = 0;
        VkBufferObj vertex_buffer(*m_device, 64, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        vk::CmdBindVertexBuffers(m_commandBuffer->handle(), 0, 1, &vertex_buffer.handle(), &offset);

        m_commandBuffer->Draw(1, 0, 0, 0);

        m_commandBuffer->EndRenderPass();
    }

    vk::EndCommandBuffer(m_commandBuffer->handle());
    m_errorMonitor->VerifyFound();

    {
        // Create and bind a vertex buffer in a reduced scope, and delete it
        // twice, the second through the destructor
        VkBufferTest buffer_test(m_device, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VkBufferTest::eDoubleDelete);
        m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkDestroyBuffer-buffer-parameter");
        buffer_test.TestDoubleDestroy();
    }
    m_errorMonitor->VerifyFound();

    m_errorMonitor->SetUnexpectedError("value of pCreateInfo->usage must not be 0");
    if (VkBufferTest::GetTestConditionValid(m_device, VkBufferTest::eInvalidMemoryOffset)) {
        // Create and bind a memory buffer with an invalid offset.
        m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkBindBufferMemory-memoryOffset-01036");
        m_errorMonitor->SetUnexpectedError(
            "If buffer was created with the VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT or VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT, "
            "memoryOffset must be a multiple of VkPhysicalDeviceLimits::minTexelBufferOffsetAlignment");
        VkBufferTest buffer_test(m_device, VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT, VkBufferTest::eInvalidMemoryOffset);
        (void)buffer_test;
        m_errorMonitor->VerifyFound();
    }

    {
        // Attempt to bind a null buffer.
        m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "UNASSIGNED-GeneralParameterError-RequiredParameter");
        VkBufferTest buffer_test(m_device, 0, VkBufferTest::eBindNullBuffer);
        (void)buffer_test;
        m_errorMonitor->VerifyFound();
    }

    {
        // Attempt to bind a fake buffer.
        m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkBindBufferMemory-buffer-parameter");
        VkBufferTest buffer_test(m_device, 0, VkBufferTest::eBindFakeBuffer);
        (void)buffer_test;
        m_errorMonitor->VerifyFound();
    }

    {
        // Attempt to use an invalid handle to delete a buffer.
        m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkFreeMemory-memory-parameter");
        VkBufferTest buffer_test(m_device, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VkBufferTest::eFreeInvalidHandle);
        (void)buffer_test;
    }
    m_errorMonitor->VerifyFound();
}

TEST_F(NegativeBuffer, VertexBufferOffset) {
    TEST_DESCRIPTION("Submit an offset past the end of a vertex buffer");

    ASSERT_NO_FATAL_FAILURE(Init());
    ASSERT_NO_FATAL_FAILURE(InitRenderTarget());
    const uint32_t maxVertexInputBindings = m_device->props.limits.maxVertexInputBindings;
    static const float vbo_data[3] = {1.f, 0.f, 1.f};
    VkConstantBufferObj vbo(m_device, sizeof(vbo_data), (const void *)&vbo_data, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    m_commandBuffer->begin();
    m_commandBuffer->BeginRenderPass(m_renderPassBeginInfo);

    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdBindVertexBuffers-pOffsets-00626");
    m_commandBuffer->BindVertexBuffer(&vbo, (VkDeviceSize)(3 * sizeof(float)), 1);  // Offset at the end of the buffer
    m_errorMonitor->VerifyFound();

    // firstBinding set over limit
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdBindVertexBuffers-firstBinding-00624");
    m_commandBuffer->BindVertexBuffer(&vbo, 0, (maxVertexInputBindings + 1));
    m_errorMonitor->VerifyFound();

    // sum of firstBinding and bindingCount set over limit
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdBindVertexBuffers-firstBinding-00625");
    m_commandBuffer->BindVertexBuffer(&vbo, 0, (maxVertexInputBindings));  // bindingCount of 1 puts it over limit
    m_errorMonitor->VerifyFound();

    m_commandBuffer->EndRenderPass();
    m_commandBuffer->end();
}

TEST_F(NegativeBuffer, IndexBufferOffset) {
    TEST_DESCRIPTION("Submit bad offsets binding the index buffer");

    AddRequiredExtensions(VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME);
    ASSERT_NO_FATAL_FAILURE(InitFramework());
    if (!AreRequiredExtensionsEnabled()) {
        GTEST_SKIP() << RequiredExtensionsNotSupported() << " not supported.";
    }
    ASSERT_NO_FATAL_FAILURE(InitState());
    ASSERT_NO_FATAL_FAILURE(InitRenderTarget());
    const uint32_t buffer_size = 32;
    VkBufferObj buffer(*m_device, buffer_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    m_commandBuffer->begin();
    m_commandBuffer->BeginRenderPass(m_renderPassBeginInfo);

    // Set offset over buffer size
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdBindIndexBuffer-offset-08782");
    m_commandBuffer->BindIndexBuffer(&buffer, buffer_size + 4, VK_INDEX_TYPE_UINT32);
    m_errorMonitor->VerifyFound();

    // Set offset to be misaligned with index buffer UINT32 type
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdBindIndexBuffer-offset-08783");
    m_commandBuffer->BindIndexBuffer(&buffer, 1, VK_INDEX_TYPE_UINT32);
    m_errorMonitor->VerifyFound();

    // Test for missing pNext struct for index buffer UINT8 type
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdBindIndexBuffer-indexType-08787");
    m_commandBuffer->BindIndexBuffer(&buffer, 1, VK_INDEX_TYPE_UINT8_EXT);
    m_errorMonitor->VerifyFound();

    m_commandBuffer->EndRenderPass();
    m_commandBuffer->end();
}

TEST_F(NegativeBuffer, IndexBuffer2Offset) {
    TEST_DESCRIPTION("Submit bad offsets binding the index buffer using vkCmdBindIndexBuffer2KHR");
    SetTargetApiVersion(VK_API_VERSION_1_1);
    AddRequiredExtensions(VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME);
    AddRequiredExtensions(VK_KHR_MAINTENANCE_5_EXTENSION_NAME);
    ASSERT_NO_FATAL_FAILURE(InitFramework());
    if (DeviceValidationVersion() < VK_API_VERSION_1_1) {
        GTEST_SKIP() << "At least Vulkan 1.1 is required";
    }
    if (!AreRequiredExtensionsEnabled()) {
        GTEST_SKIP() << RequiredExtensionsNotSupported() << " not supported.";
    }
    auto maintenance5_features = LvlInitStruct<VkPhysicalDeviceMaintenance5FeaturesKHR>();
    GetPhysicalDeviceFeatures2(maintenance5_features);
    ASSERT_NO_FATAL_FAILURE(InitState(nullptr, &maintenance5_features));

    ASSERT_NO_FATAL_FAILURE(InitRenderTarget());
    const uint32_t buffer_size = 32;
    VkBufferObj buffer(*m_device, buffer_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    m_commandBuffer->begin();
    m_commandBuffer->BeginRenderPass(m_renderPassBeginInfo);

    // Set offset over buffer size
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdBindIndexBuffer2KHR-offset-08782");
    vk::CmdBindIndexBuffer2KHR(m_commandBuffer->handle(), buffer.handle(), buffer_size + 4, VK_WHOLE_SIZE, VK_INDEX_TYPE_UINT32);
    m_errorMonitor->VerifyFound();

    // Set offset to be misaligned with index buffer UINT32 type
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdBindIndexBuffer2KHR-offset-08783");
    vk::CmdBindIndexBuffer2KHR(m_commandBuffer->handle(), buffer.handle(), 1, VK_WHOLE_SIZE, VK_INDEX_TYPE_UINT32);
    m_errorMonitor->VerifyFound();

    // Test for missing pNext struct for index buffer UINT8 type
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdBindIndexBuffer2KHR-indexType-08787");
    vk::CmdBindIndexBuffer2KHR(m_commandBuffer->handle(), buffer.handle(), 1, VK_WHOLE_SIZE, VK_INDEX_TYPE_UINT8_EXT);
    m_errorMonitor->VerifyFound();

    m_commandBuffer->EndRenderPass();
    m_commandBuffer->end();
}

TEST_F(NegativeBuffer, IndexBuffer2Size) {
    TEST_DESCRIPTION("Submit bad size binding the index buffer using vkCmdBindIndexBuffer2KHR");
    SetTargetApiVersion(VK_API_VERSION_1_1);
    AddRequiredExtensions(VK_KHR_MAINTENANCE_5_EXTENSION_NAME);
    ASSERT_NO_FATAL_FAILURE(InitFramework());
    if (DeviceValidationVersion() < VK_API_VERSION_1_1) {
        GTEST_SKIP() << "At least Vulkan 1.1 is required";
    }
    if (!AreRequiredExtensionsEnabled()) {
        GTEST_SKIP() << RequiredExtensionsNotSupported() << " not supported.";
    }
    auto maintenance5_features = LvlInitStruct<VkPhysicalDeviceMaintenance5FeaturesKHR>();
    GetPhysicalDeviceFeatures2(maintenance5_features);
    ASSERT_NO_FATAL_FAILURE(InitState(nullptr, &maintenance5_features));
    ASSERT_NO_FATAL_FAILURE(InitRenderTarget());

    const uint32_t buffer_size = 32;
    VkBufferObj buffer(*m_device, buffer_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    VkMemoryRequirements mem_reqs;
    vk::GetBufferMemoryRequirements(m_device->device(), buffer.handle(), &mem_reqs);
    m_commandBuffer->begin();
    m_commandBuffer->BeginRenderPass(m_renderPassBeginInfo);

    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdBindIndexBuffer2KHR-size-08767");
    vk::CmdBindIndexBuffer2KHR(m_commandBuffer->handle(), buffer.handle(), 4, 6, VK_INDEX_TYPE_UINT32);
    m_errorMonitor->VerifyFound();

    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdBindIndexBuffer2KHR-size-08768");
    vk::CmdBindIndexBuffer2KHR(m_commandBuffer->handle(), buffer.handle(), 4, buffer_size, VK_INDEX_TYPE_UINT32);
    m_errorMonitor->VerifyFound();

    m_commandBuffer->EndRenderPass();
    m_commandBuffer->end();
}

TEST_F(NegativeBuffer, BufferUsageFlags2) {
    TEST_DESCRIPTION("VkBufferUsageFlags2CreateInfoKHR with bad flags.");
    SetTargetApiVersion(VK_API_VERSION_1_1);
    AddRequiredExtensions(VK_KHR_MAINTENANCE_5_EXTENSION_NAME);
    ASSERT_NO_FATAL_FAILURE(InitFramework());
    if (DeviceValidationVersion() < VK_API_VERSION_1_1) {
        GTEST_SKIP() << "At least Vulkan 1.1 is required";
    }
    if (!AreRequiredExtensionsEnabled()) {
        GTEST_SKIP() << RequiredExtensionsNotSupported() << " not supported.";
    }
    auto maintenance5_features = LvlInitStruct<VkPhysicalDeviceMaintenance5FeaturesKHR>();
    GetPhysicalDeviceFeatures2(maintenance5_features);
    ASSERT_NO_FATAL_FAILURE(InitState(nullptr, &maintenance5_features));

    auto buffer_ci = LvlInitStruct<VkBufferCreateInfo>();
    buffer_ci.size = 32;
    buffer_ci.usage = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    VkBufferObj buffer(*m_device, buffer_ci);

    auto buffer_usage_flags = LvlInitStruct<VkBufferUsageFlags2CreateInfoKHR>();
    buffer_usage_flags.usage = VK_BUFFER_USAGE_2_UNIFORM_TEXEL_BUFFER_BIT_KHR | VK_BUFFER_USAGE_2_INDEX_BUFFER_BIT_KHR;

    auto buffer_view_ci = LvlInitStruct<VkBufferViewCreateInfo>(&buffer_usage_flags);
    buffer_view_ci.format = VK_FORMAT_R8G8B8A8_UNORM;
    buffer_view_ci.range = VK_WHOLE_SIZE;
    buffer_view_ci.buffer = buffer.handle();
    CreateBufferViewTest(*this, &buffer_view_ci, {"VUID-VkBufferViewCreateInfo-pNext-08780"});
}

TEST_F(NegativeBuffer, BufferUsageFlagsUsage) {
    TEST_DESCRIPTION("Use bad buffer usage flag.");
    ASSERT_NO_FATAL_FAILURE(Init());

    auto buffer_ci = LvlInitStruct<VkBufferCreateInfo>();
    buffer_ci.size = 32;
    buffer_ci.usage = 0;
    CreateBufferTest(*this, &buffer_ci, {"VUID-VkBufferCreateInfo-None-09206"});

    buffer_ci.usage = 0xBAD0000;
    CreateBufferTest(*this, &buffer_ci, {"VUID-VkBufferCreateInfo-None-09205"});
}

TEST_F(NegativeBuffer, BufferUsageFlags2Subset) {
    TEST_DESCRIPTION("VkBufferUsageFlags2CreateInfoKHR that are not a subset of the Buffer.");
    SetTargetApiVersion(VK_API_VERSION_1_1);
    AddRequiredExtensions(VK_KHR_MAINTENANCE_5_EXTENSION_NAME);
    ASSERT_NO_FATAL_FAILURE(InitFramework());
    if (DeviceValidationVersion() < VK_API_VERSION_1_1) {
        GTEST_SKIP() << "At least Vulkan 1.1 is required";
    }
    if (!AreRequiredExtensionsEnabled()) {
        GTEST_SKIP() << RequiredExtensionsNotSupported() << " not supported.";
    }
    auto maintenance5_features = LvlInitStruct<VkPhysicalDeviceMaintenance5FeaturesKHR>();
    GetPhysicalDeviceFeatures2(maintenance5_features);
    ASSERT_NO_FATAL_FAILURE(InitState(nullptr, &maintenance5_features));

    auto buffer_ci = LvlInitStruct<VkBufferCreateInfo>();
    buffer_ci.size = 32;
    buffer_ci.usage = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
    VkBufferObj buffer(*m_device, buffer_ci);

    auto buffer_usage_flags = LvlInitStruct<VkBufferUsageFlags2CreateInfoKHR>();
    buffer_usage_flags.usage = VK_BUFFER_USAGE_2_UNIFORM_TEXEL_BUFFER_BIT_KHR;

    auto buffer_view_ci = LvlInitStruct<VkBufferViewCreateInfo>(&buffer_usage_flags);
    buffer_view_ci.format = VK_FORMAT_R8G8B8A8_UNORM;
    buffer_view_ci.range = VK_WHOLE_SIZE;
    buffer_view_ci.buffer = buffer.handle();
    CreateBufferViewTest(*this, &buffer_view_ci, {"VUID-VkBufferViewCreateInfo-pNext-08781"});
}

TEST_F(NegativeBuffer, CreateBufferSize) {
    TEST_DESCRIPTION("Attempt to create VkBuffer with size of zero");

    ASSERT_NO_FATAL_FAILURE(Init());

    VkBufferCreateInfo info = LvlInitStruct<VkBufferCreateInfo>();
    info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    info.size = 0;
    CreateBufferTest(*this, &info, "VUID-VkBufferCreateInfo-size-00912");
}

TEST_F(NegativeBuffer, DedicatedAllocationBufferFlags) {
    TEST_DESCRIPTION("Verify that flags are valid with VkDedicatedAllocationBufferCreateInfoNV");

    // Positive test to check parameter_validation and unique_objects support for NV_dedicated_allocation
    AddRequiredExtensions(VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME);
    ASSERT_NO_FATAL_FAILURE(InitFramework(m_errorMonitor));
    if (!AreRequiredExtensionsEnabled()) {
        GTEST_SKIP() << RequiredExtensionsNotSupported() << " not supported";
    }
    ASSERT_NO_FATAL_FAILURE(InitState());

    VkDedicatedAllocationBufferCreateInfoNV dedicated_buffer_create_info = LvlInitStruct<VkDedicatedAllocationBufferCreateInfoNV>();
    dedicated_buffer_create_info.dedicatedAllocation = VK_TRUE;

    uint32_t queue_family_index = 0;
    VkBufferCreateInfo buffer_create_info = LvlInitStruct<VkBufferCreateInfo>();
    buffer_create_info.pNext = &dedicated_buffer_create_info;
    buffer_create_info.flags = VK_BUFFER_CREATE_SPARSE_BINDING_BIT;
    buffer_create_info.size = 1024;
    buffer_create_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    buffer_create_info.queueFamilyIndexCount = 1;
    buffer_create_info.pQueueFamilyIndices = &queue_family_index;
    CreateBufferTest(*this, &buffer_create_info, "VUID-VkBufferCreateInfo-pNext-01571");
}

TEST_F(NegativeBuffer, FillBufferCmdPoolUnsupported) {
    TEST_DESCRIPTION(
        "Use a command buffer with vkCmdFillBuffer that was allocated from a command pool that does not support graphics or "
        "compute opeartions");

    ASSERT_NO_FATAL_FAILURE(Init());

    const std::optional<uint32_t> transfer = m_device->QueueFamilyWithoutCapabilities(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
    if (!transfer) {
        GTEST_SKIP() << "Required queue families not present (non-graphics non-compute capable required)";
    }
    VkQueueObj *queue = m_device->queue_family_queues(transfer.value())[0].get();

    VkCommandPoolObj pool(m_device, transfer.value(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    VkCommandBufferObj cb(m_device, &pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, queue);
    VkBufferObj buffer(*m_device, 20, VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    cb.begin();
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdFillBuffer-apiVersion-07894");
    cb.FillBuffer(buffer.handle(), 0, 12, 0x11111111);
    m_errorMonitor->VerifyFound();
    cb.end();
}

TEST_F(NegativeBuffer, ConditionalRenderingBufferUsage) {
    TEST_DESCRIPTION("Use a buffer without conditional rendering usage when needed.");

    AddRequiredExtensions(VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME);
    ASSERT_NO_FATAL_FAILURE(InitFramework(m_errorMonitor));

    if (!AreRequiredExtensionsEnabled()) {
        GTEST_SKIP() << RequiredExtensionsNotSupported() << " not supported";
    }
    ASSERT_NO_FATAL_FAILURE(InitState());

    VkBufferCreateInfo buffer_create_info = LvlInitStruct<VkBufferCreateInfo>();
    buffer_create_info.size = 1024;
    buffer_create_info.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    VkBufferObj buffer(*m_device, buffer_create_info);

    VkConditionalRenderingBeginInfoEXT conditional_rendering_begin = LvlInitStruct<VkConditionalRenderingBeginInfoEXT>();
    conditional_rendering_begin.buffer = buffer.handle();

    m_commandBuffer->begin();
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-VkConditionalRenderingBeginInfoEXT-buffer-01982");
    vk::CmdBeginConditionalRenderingEXT(m_commandBuffer->handle(), &conditional_rendering_begin);
    m_errorMonitor->VerifyFound();
    m_commandBuffer->end();
}

TEST_F(NegativeBuffer, ConditionalRenderingOffset) {
    TEST_DESCRIPTION("Begin conditional rendering with invalid offset.");

    AddRequiredExtensions(VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME);
    ASSERT_NO_FATAL_FAILURE(InitFramework());
    if (!AreRequiredExtensionsEnabled()) {
        GTEST_SKIP() << RequiredExtensionsNotSupported() << " not supported";
    }
    m_device_extension_names.push_back(VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME);
    ASSERT_NO_FATAL_FAILURE(InitState());

    VkBufferCreateInfo buffer_create_info = LvlInitStruct<VkBufferCreateInfo>();
    buffer_create_info.size = 128;
    buffer_create_info.usage = VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT;
    VkBufferObj buffer(*m_device, buffer_create_info);

    VkConditionalRenderingBeginInfoEXT conditional_rendering_begin = LvlInitStruct<VkConditionalRenderingBeginInfoEXT>();
    conditional_rendering_begin.buffer = buffer.handle();
    conditional_rendering_begin.offset = 3;

    m_commandBuffer->begin();

    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-VkConditionalRenderingBeginInfoEXT-offset-01984");
    vk::CmdBeginConditionalRenderingEXT(m_commandBuffer->handle(), &conditional_rendering_begin);
    m_errorMonitor->VerifyFound();

    conditional_rendering_begin.offset = buffer_create_info.size;
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-VkConditionalRenderingBeginInfoEXT-offset-01983");
    vk::CmdBeginConditionalRenderingEXT(m_commandBuffer->handle(), &conditional_rendering_begin);
    m_errorMonitor->VerifyFound();

    m_commandBuffer->end();
}

TEST_F(NegativeBuffer, BeginConditionalRendering) {
    TEST_DESCRIPTION("Begin conditional rendering when it is already active.");

    AddRequiredExtensions(VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME);
    ASSERT_NO_FATAL_FAILURE(InitFramework(m_errorMonitor));
    if (!AreRequiredExtensionsEnabled()) {
        GTEST_SKIP() << RequiredExtensionsNotSupported() << " not supported";
    }
    ASSERT_NO_FATAL_FAILURE(InitState());

    VkBufferCreateInfo buffer_create_info = LvlInitStruct<VkBufferCreateInfo>();
    buffer_create_info.size = 32;
    buffer_create_info.usage = VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT;
    VkBufferObj buffer(*m_device, buffer_create_info);

    VkConditionalRenderingBeginInfoEXT conditional_rendering_begin = LvlInitStruct<VkConditionalRenderingBeginInfoEXT>();
    conditional_rendering_begin.buffer = buffer.handle();

    m_commandBuffer->begin();
    vk::CmdBeginConditionalRenderingEXT(m_commandBuffer->handle(), &conditional_rendering_begin);
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdBeginConditionalRenderingEXT-None-01980");
    vk::CmdBeginConditionalRenderingEXT(m_commandBuffer->handle(), &conditional_rendering_begin);
    m_errorMonitor->VerifyFound();
    vk::CmdEndConditionalRenderingEXT(m_commandBuffer->handle());
    m_commandBuffer->end();
}

TEST_F(NegativeBuffer, CompletelyOverlappingBufferCopy) {
    TEST_DESCRIPTION("Test copying between buffers with completely overlapping source and destination regions.");
    ASSERT_NO_FATAL_FAILURE(Init());

    VkBufferCopy copy_info;
    copy_info.srcOffset = 0;
    copy_info.dstOffset = 0;
    copy_info.size = 256;
    VkBufferObj buffer(*m_device, copy_info.size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 0);

    VkBufferObj buffer_shared_memory;
    buffer_shared_memory.init_no_mem(*m_device, buffer.create_info());
    buffer_shared_memory.bind_memory(buffer.memory(), 0u);

    m_commandBuffer->begin();

    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdCopyBuffer-pRegions-00117");
    vk::CmdCopyBuffer(m_commandBuffer->handle(), buffer.handle(), buffer.handle(), 1, &copy_info);

    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdCopyBuffer-pRegions-00117");
    vk::CmdCopyBuffer(m_commandBuffer->handle(), buffer.handle(), buffer_shared_memory.handle(), 1, &copy_info);

    m_commandBuffer->end();

    m_errorMonitor->VerifyFound();
}

TEST_F(NegativeBuffer, CopyingInterleavedRegions) {
    TEST_DESCRIPTION("Test copying between interleaved source and destination regions.");
    ASSERT_NO_FATAL_FAILURE(Init());

    VkBufferCopy copy_infos[4];
    copy_infos[0].srcOffset = 0;
    copy_infos[0].dstOffset = 4;
    copy_infos[0].size = 4;
    copy_infos[1].srcOffset = 8;
    copy_infos[1].dstOffset = 12;
    copy_infos[1].size = 4;
    copy_infos[2].srcOffset = 16;
    copy_infos[2].dstOffset = 20;
    copy_infos[2].size = 4;
    copy_infos[3].srcOffset = 24;
    copy_infos[3].dstOffset = 28;
    copy_infos[3].size = 4;

    VkBufferObj buffer(*m_device, 32, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 0);

    VkBufferObj buffer_shared_memory;
    buffer_shared_memory.init_no_mem(*m_device, buffer.create_info());
    buffer_shared_memory.bind_memory(buffer.memory(), 0u);

    m_commandBuffer->begin();

    vk::CmdCopyBuffer(m_commandBuffer->handle(), buffer.handle(), buffer.handle(), 4, copy_infos);
    vk::CmdCopyBuffer(m_commandBuffer->handle(), buffer.handle(), buffer_shared_memory.handle(), 4, copy_infos);

    copy_infos[2].dstOffset = 21;
    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdCopyBuffer-pRegions-00117");
    vk::CmdCopyBuffer(m_commandBuffer->handle(), buffer.handle(), buffer.handle(), 4, copy_infos);

    m_errorMonitor->SetDesiredFailureMsg(kErrorBit, "VUID-vkCmdCopyBuffer-pRegions-00117");
    vk::CmdCopyBuffer(m_commandBuffer->handle(), buffer.handle(), buffer_shared_memory.handle(), 4, copy_infos);
    m_errorMonitor->VerifyFound();

    m_commandBuffer->end();
}

TEST_F(NegativeBuffer, MaxBufferSize) {
    TEST_DESCRIPTION("check limit of maxBufferSize");

    SetTargetApiVersion(VK_API_VERSION_1_1);
    AddRequiredExtensions(VK_KHR_MAINTENANCE_4_EXTENSION_NAME);
    ASSERT_NO_FATAL_FAILURE(InitFramework());
    if (DeviceValidationVersion() < VK_API_VERSION_1_1) {
        GTEST_SKIP() << "At least Vulkan 1.1 is required";
    }
    if (!AreRequiredExtensionsEnabled()) {
        GTEST_SKIP() << RequiredExtensionsNotSupported() << " not supported";
    }

    auto maintenance4_features = LvlInitStruct<VkPhysicalDeviceMaintenance4FeaturesKHR>();
    GetPhysicalDeviceFeatures2(maintenance4_features);
    if (!maintenance4_features.maintenance4) {
        GTEST_SKIP() << "VkPhysicalDeviceMaintenance4FeaturesKHR::maintenance4 is required but not enabled.";
    }
    ASSERT_NO_FATAL_FAILURE(InitState(nullptr, &maintenance4_features));

    auto maintenance4_properties = LvlInitStruct<VkPhysicalDeviceMaintenance4Properties>();
    GetPhysicalDeviceProperties2(maintenance4_properties);

    const VkDeviceSize max_test_size = (1ull << 32);
    if (maintenance4_properties.maxBufferSize >= max_test_size) {
        GTEST_SKIP() << "maxBufferSize too large to test";
    }

    auto buffer_create_info = LvlInitStruct<VkBufferCreateInfo>();
    buffer_create_info.size = maintenance4_properties.maxBufferSize + 1;
    buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    CreateBufferTest(*this, &buffer_create_info, "VUID-VkBufferCreateInfo-size-06409");
}