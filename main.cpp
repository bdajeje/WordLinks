#include <iostream>
#include <functional>
#include <memory>

#include "links_graph.hpp"

typedef std::unique_ptr<LinksGraph> GraphPtr;
typedef std::function<void(GraphPtr&)> MenuMethod;

struct MenuEntry
{
  MenuEntry(const std::string& text, MenuMethod method)
    : text{text}
    , method{method}
  {}

  std::string text;
  MenuMethod method;
};

std::unique_ptr<LinksGraph> graph;

void read(GraphPtr& graph)
{
  std::cout << "\nText path: " << std::flush;
  std::string text_path;
  std::cin >> text_path;
  graph->read(text_path);
}

void save(const GraphPtr& graph)
{
  graph->save();
}

void describeEntity(const GraphPtr& graph)
{
  std::cout << "\nEntity name: " << std::flush;
  std::string entity_name;
  std::cin >> entity_name;
  std::cout << graph->describe(entity_name) << std::endl;
}

void describeGraph(const GraphPtr& graph)
{
  std::cout << graph->describe() << std::endl;
}

void entityTopRelations(const GraphPtr& graph)
{
  std::cout << "\nEntity name: " << std::flush;
  std::string entity_name;
  std::cin >> entity_name;
  EntityPtr entity = graph->get(entity_name);
  if( !entity )
    std::cerr << "entity not found" << std::endl;

  std::cout << "Number relations: " << std::flush;
  unsigned int nbr_relations;
  std::cin >> nbr_relations;
  const std::vector<Link> top_relations = entity->topRelations(nbr_relations);
  for( const Link& relation : top_relations )
    std::cout << relation.weight << ": " << relation.entity->name() << "\n";
  std::cout << std::flush;
}

static const std::vector<MenuEntry> menu_entries = {
  {"Read text", std::bind(read, std::ref(graph))},
  {"Save", std::bind(save, std::ref(graph))},
  {"Describe entity", std::bind(describeEntity, std::ref(graph))},
  {"Describe graph", std::bind(describeGraph, std::ref(graph))},
  {"Get entity top relations", std::bind(entityTopRelations, std::ref(graph))}
};

std::string createMenuLine(const std::string line)
{
  return "|" + line + "|\n";
}

std::string createMenu()
{
  static const std::string title {" Make a choice "};

  std::string menu_str {"\n"};

  // Find longest entry
  size_t length {title.length() + 4};
  for( const MenuEntry& menu_entry : menu_entries )
  {
    if( menu_entry.text.length() > length )
      length = menu_entry.text.length();
  }

  // Add spacing
  const size_t choice_number_max_length {menu_entries.size() / 10 + 1};
  length += choice_number_max_length + 4;
  if( length % 2 == 0 )
    length++;

  // Display title
  const size_t nbr_decoration = (length - title.length()) / 2;
  const std::string decoration(nbr_decoration, '-');
  menu_str += createMenuLine( decoration + title + decoration );

  // Display entries
  for( size_t i = 0; i < menu_entries.size(); ++i )
  {
    std::string choice_number = std::to_string(i+1);
    if( choice_number.length() < choice_number_max_length )
      choice_number += std::string(choice_number_max_length - choice_number.length(), ' ');
    std::string line {" " + choice_number + "- " + menu_entries[i].text};
    line += std::string(length - line.length(), ' ');
    menu_str += createMenuLine( line );
  }

  // Display end of menu
  menu_str += createMenuLine( std::string(length, '-') );

  // Display user choice
  menu_str +=  " Choice: ";

  return menu_str;
}

void displayMenu()
{
  static std::string menu_str;

  if( menu_str.empty() )
    menu_str = createMenu();

  std::cout << menu_str << std::flush;
}

int main(int argc, char** argv)
{
  // Check arguments
  if( argc < 2 )
  {
    std::cerr << "Please provide an ontologie file path" << std::endl;
    return EXIT_FAILURE;
  }

  // Create graph from ontologie
  graph.reset( new LinksGraph(argv[1]) );

  // Read command line provided filepath
  for(size_t i = 2; i < argc; ++i)
    graph->read( argv[i] );

  while(true)
  {
    displayMenu();

    // Read user input
    short user_input;
    std::cin >> user_input;
    user_input--;

    // Find menu entry
    if( user_input >= menu_entries.size() )
      continue;

    // Execute menu entry related method
    menu_entries[user_input].method(graph);
  }

  return EXIT_SUCCESS;
}

