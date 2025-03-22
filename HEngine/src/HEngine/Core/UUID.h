#pragma once

#include <functional> 

namespace HEngine
{

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};
}

namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<HEngine::UUID>
	{
		std::size_t operator()(const HEngine::UUID& uuid) const
		{
			return std::hash<uint64_t>()(static_cast<uint64_t>(uuid));
		}
	};

}