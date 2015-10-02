#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <vector>
#include <memory>
#include <map>

class Entity;
typedef std::shared_ptr<Entity> EntityPtr;

/*! Represents a link between two entities */
struct Link
{
  Link(const EntityPtr& entity)
    : entity {entity}
  {}

  /*! Target entity */
  EntityPtr entity;

  /*! Weight of the link */
  unsigned int weight {0};
};

class Entity final
{
  public:

    Entity(const std::string& name);

    const std::string& name() const { return _name; }
    std::string describe() const;
    const std::map<std::string, Link>& links() const { return _links; }

    void linkTo( const EntityPtr& entity );
    void linkTo( const std::vector<EntityPtr>& entities );

    std::vector<Link> topRelations(size_t nbr_relations) const;

  private:

    std::string _name;
    std::map<std::string, Link> _links;
};

#endif // ENTITY_HPP
