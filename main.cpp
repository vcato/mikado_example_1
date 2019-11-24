#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "gui.hpp"
#include "applicationexception.hpp"
#include "filedb.hpp"

#define REPLACE_FILEDB_WITH_DATABASE_INTERFACE 0
#define CHANGE_PARAMETER_TO_ADAPTER 0
#define PASS_ADAPTER 0
#define ADD_LOAD_METHOD_TO_ADAPTER 0
#define ADD_FILEDB_MEMBER_TO_ADAPTER 1

using std::vector;
using std::string;
using std::make_unique;
using std::cerr;


struct Database {
  virtual std::vector<std::string> load(const std::string &/*name*/) = 0;
};


struct FileDBAdapter : Database {
#if ADD_FILEDB_MEMBER_TO_ADAPTER
  FileDB &file_db;
#endif

#if ADD_LOAD_METHOD_TO_ADAPTER
  std::vector<std::string> load(const std::string &/*name*/) override
  {
    return file_db.load(name);
  }
#endif
};


class UI {
  public:
#if !CHANGE_PARAMETER_TO_ADAPTER
    UI(FileDB &);
#else
    UI(FileDBAdapter &);
#endif

    void showLogin()
    {
      vector<string> users = database.load("users");
      addLoginSelector(users);
      addButtons();
      frame.setSize(800, 600);
      frame.setVisible(true);
    }

  private:
    Frame frame;
#if !REPLACE_FILEDB_WITH_DATABASE_INTERFACE
    FileDB &database;
#else
    Database &database;
#endif

    void addLoginSelector(const vector<string> &users)
    {
      auto combo_ptr = make_unique<ComboBox>(users);
      frame.setLayout(make_unique<GridBagLayout>());
      frame.add(move(combo_ptr));
    }

    void addButtons()
    {
    }
};


class App {
  public:
    App()
#if !PASS_ADAPTER
    : ui(database)
#else
    : ui(database_adapter)
#endif
    {
    }

    void launch()
    {
      ui.showLogin();
    }

    static const string& getStorageFile()
    {
      return store_path;
    }

    static void setStorageFile(const string &store_path)
    {
      App::store_path = store_path;
    }

  private:
    FileDB database;
#if PASS_ADAPTER
#if !ADD_FILEDB_MEMBER_TO_ADAPTER
    FileDBAdapter database_adapter;
#else
    FileDBAdapter database_adapter{database};
#endif
#endif
    UI ui;
    static inline string store_path;
};


class Launcher {
  public:
    static void main(const vector<string> &argv)
    {
      try {
        App::setStorageFile(argv[0]);
        App app;
        app.launch();
      }
      catch (ApplicationException &e) {
        cerr << "Could not start application.\n";
        e.printStackTrace();
      }
    }
};



#if !CHANGE_PARAMETER_TO_ADAPTER
UI::UI(FileDB &database)
#else
UI::UI(FileDBAdapter &database)
#endif
: database(database)
{
  database.setStore(App::getStorageFile());
}



int main(int argc, char **argv)
{
  Launcher::main(vector<string>(argv+1, argv+argc));
}
