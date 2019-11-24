#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "gui.hpp"
#include "applicationexception.hpp"
#include "filedb.hpp"

using std::vector;
using std::string;
using std::make_unique;
using std::cerr;


struct Database {
  virtual std::vector<std::string> load(const std::string &/*name*/) = 0;
};


struct FileDBAdapter : Database {
  FileDB &file_db;

  FileDBAdapter(FileDB &file_db)
  : file_db(file_db)
  {
  }

  std::vector<std::string> load(const std::string &name) override
  {
    return file_db.load(name);
  }
};


class UI {
  public:
    UI(Database &);

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
    Database &database;

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
    App(Database &database)
    : ui(database)
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
    UI ui;
    static inline string store_path;
};


class Launcher {
  public:
    static void main(const vector<string> &argv)
    {
      try {
        App::setStorageFile(argv[0]);
        FileDB database;
        database.setStore(App::getStorageFile());
        FileDBAdapter database_adapter(database);
        App app(database_adapter);
        app.launch();
      }
      catch (ApplicationException &e) {
        cerr << "Could not start application.\n";
        e.printStackTrace();
      }
    }
};



UI::UI(Database &database)
: database(database)
{
}



int main(int argc, char **argv)
{
  Launcher::main(vector<string>(argv+1, argv+argc));
}
