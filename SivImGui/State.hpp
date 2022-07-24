#pragma once
#include "Common.hpp"

namespace GUI
{
	class StateStorage;

	struct State
	{
		StateStorage* storage;
	};

	class StateStorage
	{
	public:

		template<class StateType>
		StateType& get(size_t id, int32 currentFrame = Scene::FrameCount())
		{
			static_assert(std::is_base_of_v<State, StateType>);

			Key key{
				.typeInfo = typeid(StateType),
				.stateId = id,
			};

			auto itr = map.find(key);
			if (itr == map.end())
			{
				StateType* data = new StateType();
				data->storage = this;
				map.emplace(key, Value{
					.updatedFrame = currentFrame,
					.data = std::unique_ptr<StateType>(data),
				});
				return *data;
			}

			Value& value = itr->second;

			if (value.updatedFrame < currentFrame)
			{
				value.updatedFrame = currentFrame;
				return *reinterpret_cast<StateType*>(value.data.get());
			}

			throw Error(U"Widgets with the same ID cannot be created");
		}

		void clean(int32 currentFrame = Scene::FrameCount())
		{
			std::erase_if(map, [=](const MapType::value_type& v) { return v.second.updatedFrame < currentFrame; });
		}

	private:

		struct Key
		{
			const type_info& typeInfo;

			size_t stateId;

			bool operator==(const Key& other) const
			{
				return typeInfo == other.typeInfo && stateId == other.stateId;
			}
		};

		struct Value
		{
			int32 updatedFrame;

			std::unique_ptr<State> data;
		};

		using MapType = std::unordered_map < Key, Value, decltype([](const Key& key) { return key.typeInfo.hash_code() ^ key.stateId; }) > ;

		MapType map;
	};
}
