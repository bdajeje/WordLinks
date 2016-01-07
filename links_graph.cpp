#include "links_graph.hpp"

#include <iostream>

#include <boost/algorithm/string.hpp>

#include "files.hpp"

const std::string LinksGraph::_serializer_delimiter {"|"};

LinksGraph::LinksGraph(const std::string& ontologie_path)
  : _ontology_path {ontologie_path}
{
  load(ontologie_path);
}

std::string LinksGraph::describe(const std::string& entity_name) const
{
  auto entity_it = _entities.find(entity_name);

  if( entity_it == _entities.end() )
    return "Entity '" + entity_name + "' not found";

  return entity_it->second->describe();
}

std::string LinksGraph::describe() const
{
  std::string result;

  for(auto it = _entities.begin(); it != _entities.end(); ++it)
    result += it->second->describe() + "\n";

  return result;
}

bool LinksGraph::read(const std::string& filepath)
{
  // Read file
  std::string content;
  if( !utils::files::read(filepath, content) )
  {
    std::cerr << "File can't be read: " << filepath << std::endl;
    return false;
  }

  std::cout << "Reading " << filepath << "..." << std::endl;

  const size_t start_nbr_entities = _entities.size();

  // Split by sentences
  std::vector<std::string> sentences;
  boost::algorithm::split( sentences, content, boost::is_any_of(".?!;()") );

  // For each sentence, split by words then add them into graph
  unsigned long nbr_words {0};
  for( const std::string& sentence : sentences )
  {
    std::vector<std::string> words;
    boost::algorithm::split( words, sentence, boost::is_any_of(" ") );

    // Convert words to entities
    std::vector<EntityPtr> entities;
    entities.reserve(words.size());
    for(std::string& word : words )
    {
      // Remove extra spaces
      boost::algorithm::trim(word);

      // Remove possible special serialize char delimiter from word to prevent breaking serializing process
      boost::algorithm::replace_all(word, _serializer_delimiter, "");

      // Empty word, ignore
      if( word.empty() )
        continue;

      nbr_words++;

      // Lower case word so we don't care about different word cases
      boost::algorithm::to_lower(word);

      entities.push_back( findOrCreateEntity(word) );
    }

    // Links entities between them
    for( const EntityPtr& entity : entities )
      entity->linkTo( entities );
  }

  std::cout << "Read " << nbr_words << " words" << std::endl;
  std::cout << "Added " << (_entities.size() - start_nbr_entities) << " new entities" << std::endl;

  return true;
}

EntityPtr LinksGraph::get(const std::string& name) const
{
  // Find entity
  auto entity_it = _entities.find(name);
  if( entity_it != _entities.end() )
    return entity_it->second;

  return nullptr;
}

EntityPtr& LinksGraph::findOrCreateEntity( const std::string& name )
{
  // Find entity
  auto entity_it = _entities.find(name);
  if( entity_it != _entities.end() )
    return entity_it->second;

  // Create entity
  return std::get<0>(_entities.emplace( name, std::make_shared<Entity>(name) ))->second;
}

bool LinksGraph::save() const
{
  std::cout << "Saving " << _entities.size() << " entities..." << std::endl;

  // Create result
  std::string result;
  for(auto it = _entities.begin(); it != _entities.end(); ++it)
  {
    // Add entity info
    const EntityPtr& entity = it->second;
    result += entity->name();

    // Add entity links
    const auto& entity_links = entity->links();
    for(auto it_link = entity_links.begin(); it_link != entity_links.end(); ++it_link)
    {
      const EntityPtr& linked_entity = it_link->second.entity;
      result += _serializer_delimiter + linked_entity->name();
    }

    result += '\n';
  }

  // Save result to file
  if( !utils::files::create(_ontology_path, result, true) )
  {
    std::cerr << "Can't save graph to file: " << _ontology_path << std::endl;
    return false;
  }

  std::cout << "DONE" << std::endl;
  return true;
}

bool LinksGraph::load(const std::string& ontology_path)
{
  if( !utils::files::exists(ontology_path) )
    return false;

  std::cout << "Loading ontology from '" << ontology_path << "'..." << std::endl;

  std::string result;
  if( !utils::files::read(ontology_path, result) )
  {
    std::cerr << "Can't read file: " << ontology_path << std::endl;
    return false;
  }

  // Split result by lines
  std::vector<std::string> lines;
  boost::algorithm::split(lines, result, boost::is_any_of("\n"));

  // Read each line
  std::vector<std::string> parts;
  for( const std::string& line : lines )
  {
    parts.clear();
    boost::algorithm::split(parts, line, boost::is_any_of(_serializer_delimiter));

    // Create entity
    EntityPtr& entity = findOrCreateEntity(parts[0]);

    // Add links to entity
    for( size_t i = 1; i < parts.size(); ++i )
    {
      const std::string& link_name = parts[i];
      const EntityPtr& link_entity = findOrCreateEntity( link_name );
      entity->linkTo(link_entity);
    }
  }

  std::cout << _entities.size() << " entities loaded" << std::endl;

  return true;
}

