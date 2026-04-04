#pragma once

#include "Math.hpp"
#include "BlockType.hpp"
#include "Noise.hpp"
#include "Structure.hpp"
#include <functional>

/*
	Abstract biome class, needs continentalness, erosion... for generation, just need to implement get_height and get_block when implementing a biome
*/
struct	Biome
{
	public:
		Range	continentalness;
		Range	erosion;
		Range	riverness;

		Range	temperature;
		Range	humidity;

		float	tree_probab = -1;
	private:
		std::vector<std::pair<float, Structure*>>	_structures;
		std::vector<std::pair<float, Structure*>>	_surface_features;
	protected:
		void	registerSurfaceFeature(float spawn_prob, Structure *s)
		{
			_surface_features.push_back(std::make_pair(spawn_prob, s));
		}
		void	registerStructure(float spawn_prob, Structure *s)
		{
			_structures.push_back(std::make_pair(spawn_prob, s));
		}
	public:
		Biome(const Range &continentalness, const Range &erosion, const Range &riverness, const Range &temperature, const Range &humidity)
		: continentalness(continentalness), erosion(erosion), riverness(riverness), temperature(temperature), humidity(humidity)
		{

		}
		virtual ~Biome() {}

		// Returns height for biome gen
		virtual float			get_height(const Vec2i &pos) = 0;
		// Returns block at pos with biome parameters
		virtual BlockStateId	get_block(WorldVec3i pos, int world_height) = 0;

		Structure	*get_surface_feature(WorldVec3i gen_pos)
		{
			if (!_surface_features.size())
				return (NULL);
			int	i = 0;
			for (auto &p : _surface_features)
				if (rand3dTo1d(gen_pos + i++) <= p.first)
					return (p.second);
			return (NULL);
		}
		Structure	*get_structure(WorldVec3i gen_pos)
		{
			if (!_structures.size())
				return (NULL);
			int	i = 0;
			for (auto &p : _structures)
				if (rand3dTo1d(gen_pos + i++) <= p.first)
					return (p.second);
			return (NULL);
		}

		static float	get_continentalness(const Vec2f &pos)
		{
			return (noise(pos, 0.00125, 2, 6));
		}
		static float	get_erosion(const Vec2f &pos)
		{
			return (noise(pos, 0.00635, 2, 3));
		}
		static float	get_temperature(const Vec2f &pos)
		{
			return (noise(pos, 0.0015, 2, 3));
		}

		static void	get_biome(const Vec2i &pos, std::shared_ptr<Biome> &dominant_biome, float &final_height)
		{
			extern std::vector<std::shared_ptr<Biome>>	ALL_BIOMES;

			float	continentalness = get_continentalness(Vec2f(pos.x, pos.y));
			float	erosion = get_erosion(Vec2f(pos.x, pos.y));
			float	temperature = get_temperature(Vec2f(pos.x, pos.y));

			std::vector<std::pair<float, std::shared_ptr<Biome>>>	biome_weights;
			for (auto &biome : ALL_BIOMES)
			{
				float	weight = 0;

				weight += distance_to_range(continentalness, biome->continentalness);
				weight += distance_to_range(erosion, biome->erosion);
				weight += distance_to_range(temperature, biome->temperature);

				biome_weights.push_back(std::make_pair(weight, biome));
			}

			float sharpness = 64.0f;
			for (auto &[w, b] : biome_weights)
				w = exp(-w * sharpness);

			float total = 0.0f;
			for (auto [w, b] : biome_weights)
				total += w;
			for (auto &[w, b] : biome_weights)
				w /= total;

			float	biggest_weight = 0;

			final_height = 0.0f;
			for (uint64_t i = 0; i < biome_weights.size(); i++)
			{
				if (biome_weights[i].first > 0.001)
				{
					std::shared_ptr<Biome>	biome = biome_weights[i].second;
					float	biome_height = biome->get_height(Vec2i(pos.x, pos.y));

					final_height += biome_height * biome_weights[i].first;
				}

				if (biome_weights[i].first > biggest_weight)
				{
					biggest_weight = biome_weights[i].first;
					dominant_biome = biome_weights[i].second;
				}
			}
		}
};
