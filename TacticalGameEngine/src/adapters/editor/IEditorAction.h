#pragma once

#include "editor-state.h"

namespace Engine::Adapters::Editor
{

	class IEditorAction
	{
	protected:
		std::shared_ptr<Core::Interfaces::ILogger> logger_;

	public:
		IEditorAction(std::shared_ptr<Core::Interfaces::ILogger> logger) : logger_(logger) {}

		virtual ~IEditorAction() = default;

		IEditorAction(const IEditorAction &) = delete;

		IEditorAction &operator=(const IEditorAction &) = delete;

		IEditorAction(IEditorAction &&other) : logger_(std::move(other.logger_)) {}

		IEditorAction &operator=(IEditorAction &&other)
		{
			if (this != &other)
			{
				logger_ = std::move(other.logger_);
			}
			return *this;
		}

		virtual void execute(EditorState &state) = 0;
		virtual std::string getName() const = 0;
	};

}