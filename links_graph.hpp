#ifndef LINKSGRAPH_HPP
#define LINKSGRAPH_HPP

#include <string>
#include <map>
#include <chrono>

#include "entity.hpp"

//struct ReadInfo
//{
//  /*! Did the read successfull? */
//  bool success {false};

//  /*! Number of words found */
//  unsigned int nbr_words {0};

//  /*! Number of entities added into the Graph */
//  unsigned int nbr_added_entities {0};

//  /*! Elapsed time to load entities */
//  std::chrono::milliseconds load_duration;
//};

class LinksGraph final
{
  public:

    LinksGraph(const std::string& ontology_path);

    /*! Describe all entities and links */
    std::string describe() const;

    /*! Describe a specific entity */
    std::string describe(const std::string& entity_name) const;

    /*! Read a text and add it to the graph */
    bool read(const std::string& filepath);

    /*! Read a text and add it to the graph */
    bool save() const;

    /*! Get an entity by its name
     *  \note return a null entity if doens't exist
     */
    EntityPtr get(const std::string& name) const;

  private:

    /*! Load ontology into graph */
    bool load(const std::string& ontology_path);

    /*! Find already existing entity or create it if doesn't exist yet from name */
    EntityPtr& findOrCreateEntity(const std::string& name );

  private:

    std::string _ontology_path;
    std::map<std::string, EntityPtr> _entities;

    static const std::string _serializer_delimiter;
};

#endif // LINKSGRAPH_HPP
