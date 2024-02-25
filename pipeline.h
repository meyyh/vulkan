#pragma once
#include "config.h"
#include "shaders.h"

namespace vkInit {

	/**
		holds the data structures used to create a pipeline
	*/
	struct GraphicsPipelineInBundle {
		vk::Device device;
		std::string vertexFilepath;
		std::string fragmentFilepath;
		vk::Extent2D swapchainExtent;
		vk::Format swapchainImageFormat;
	};

	/**
		Used for returning the pipeline, along with associated data structures,
		after creation.
	*/
	struct GraphicsPipelineOutBundle {
		vk::PipelineLayout layout;
		vk::RenderPass renderpass;
		vk::Pipeline pipeline;
	};

    /**
		Make a graphics pipeline, along with renderpass and pipeline layout

		\param specification the struct holding input data, as specified at the top of the file.
		\param debug whether the system is running in debug mode
		\returns the bundle of data structures created
	*/
	GraphicsPipelineOutBundle make_graphics_pipeline(GraphicsPipelineInBundle& specification, bool debug) {
        /*
		* Build and return a graphics pipeline based on the given info.
		*/

		//The info for the graphics pipeline
		vk::GraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.flags = vk::PipelineCreateFlags();

		//Shader stages, to be populated later
		std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;

		//Vertex Input
		vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		pipelineInfo.pVertexInputState = &vertexInputInfo;

		//Input Assembly
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
		inputAssemblyInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
		inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;
		pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;

        //Vertex Shader
		if (debug) {
			std::cout << "Create vertex shader module" << std::endl;
		}
		vk::ShaderModule vertexShader = vkUtil::createModule(
			specification.vertexFilepath, specification.device, debug
		);
		vk::PipelineShaderStageCreateInfo vertexShaderInfo = {};
		vertexShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
		vertexShaderInfo.stage = vk::ShaderStageFlagBits::eVertex;
		vertexShaderInfo.module = vertexShader;
		vertexShaderInfo.pName = "main";
		shaderStages.push_back(vertexShaderInfo);

		//Viewport and Scissor
		vk::Viewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)specification.swapchainExtent.width;
		viewport.height = (float)specification.swapchainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vk::Rect2D scissor = {};
		scissor.offset.x = 0.0f;
		scissor.offset.y = 0.0f;
		scissor.extent = specification.swapchainExtent;
		vk::PipelineViewportStateCreateInfo viewportState = {};
		viewportState.flags = vk::PipelineViewportStateCreateFlags();
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;
		pipelineInfo.pViewportState = &viewportState;

		//Rasterizer
		vk::PipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.flags = vk::PipelineRasterizationStateCreateFlags();
		rasterizer.depthClampEnable = VK_FALSE; //discard out of bounds fragments, don't clamp them
		rasterizer.rasterizerDiscardEnable = VK_FALSE; //This flag would disable fragment output
		rasterizer.polygonMode = vk::PolygonMode::eFill;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = vk::CullModeFlagBits::eBack;
		rasterizer.frontFace = vk::FrontFace::eClockwise;
		rasterizer.depthBiasEnable = VK_FALSE; //Depth bias can be useful in shadow maps.
		pipelineInfo.pRasterizationState = &rasterizer;

		//Fragment Shader
		if (debug) {
			std::cout << "Create fragment shader module" << std::endl;
		}
		vk::ShaderModule fragmentShader = vkUtil::createModule(
			specification.fragmentFilepath, specification.device, debug
		);
		vk::PipelineShaderStageCreateInfo fragmentShaderInfo = {};
		fragmentShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
		fragmentShaderInfo.stage = vk::ShaderStageFlagBits::eFragment;
		fragmentShaderInfo.module = fragmentShader;
		fragmentShaderInfo.pName = "main";
		shaderStages.push_back(fragmentShaderInfo);
		//Now both shaders have been made, we can declare them to the pipeline info
		pipelineInfo.stageCount = shaderStages.size();
		pipelineInfo.pStages = shaderStages.data();

		//Multisampling
		vk::PipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.flags = vk::PipelineMultisampleStateCreateFlags();
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
		pipelineInfo.pMultisampleState = &multisampling;

		//Color Blend
		vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
		colorBlendAttachment.blendEnable = VK_FALSE;
		vk::PipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.flags = vk::PipelineColorBlendStateCreateFlags();
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = vk::LogicOp::eCopy;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;
		pipelineInfo.pColorBlendState = &colorBlending;

		//Pipeline Layout
		if (debug) {
			std::cout << "Create Pipeline Layout" << std::endl;
		}
		vk::PipelineLayout pipelineLayout = make_pipeline_layout(specification.device, debug);
		pipelineInfo.layout = pipelineLayout;

		//Renderpass
		if (debug) {
			std::cout << "Create RenderPass" << std::endl;
		}
		vk::RenderPass renderpass = make_renderpass(
			specification.device, specification.swapchainImageFormat, debug
		);
		pipelineInfo.renderPass = renderpass;
		pipelineInfo.subpass = 0;

		//Extra stuff
		pipelineInfo.basePipelineHandle = nullptr;

		//Make the Pipeline
		if (debug) {
			std::cout << "Create Graphics Pipeline" << std::endl;
		}
		vk::Pipeline graphicsPipeline;
		try {
			graphicsPipeline = (specification.device.createGraphicsPipeline(nullptr, pipelineInfo)).value;
		}
		catch (vk::SystemError err) {
			if (debug) {
				std::cout << "Failed to create Pipeline" << std::endl;
			}
		}

		GraphicsPipelineOutBundle output;
		output.layout = pipelineLayout;
		output.renderpass = renderpass;
		output.pipeline = graphicsPipeline;

		//Finally clean up by destroying shader modules
		specification.device.destroyShaderModule(vertexShader);
		specification.device.destroyShaderModule(fragmentShader);
        return output;
    }
}