#include "entity.hpp"

#include <algorithm>

#include "convertions.hpp"

Entity::Entity(const std::string& name)
  : _name {name}
{}

void Entity::linkTo( const EntityPtr& entity )
{
  // Ignore same entity
  if( entity->name() == _name )
    return;

  // Create link if doesn't exist
  auto found_it = _links.find(entity->name());
  if( found_it == _links.end() )
    _links.emplace( entity->name(), Link{entity} );

  // Raise link weight
  _links.at(entity->name()).weight++;
}

void Entity::linkTo( const std::vector<EntityPtr>& entities )
{
  for( const EntityPtr& entity : entities )
    linkTo(entity);
}

std::string Entity::describe() const
{
  std::string result {_name + ": "};

  for(auto it = _links.begin(); it != _links.end(); ++it)
    result += "\n" + it->first + " (" + std::to_string(it->second.weight) + ")";

  return result;
}

std::vector<Link> Entity::topRelations(size_t nbr_relations) const
{
  nbr_relations = std::min(nbr_relations, _links.size());

  std::vector<Link> result;
  result.reserve( nbr_relations );

  // Convert links map to vector
  std::vector<Link> links = utils::toVector<std::string, Link>(_links);

  // Sort links by weights
  std::sort( links.begin(), links.end(), [] (const Link& a, const Link& b) -> bool {
    return a.weight > b.weight;
  } );

  // Add them to result
  result.insert(result.end(), links.begin(), links.begin() + nbr_relations);

  return result;
}
