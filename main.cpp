#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "gui.hpp"
#include "applicationexception.hpp"
#include "filedb.hpp"

#define REPLACE_FILEDB_WITH_DATABASE_INTERFACE 0
#define CHANGE_DATABASE_TO_REFERENCE 0
#define INITIALIZE_REFERENCE 0
#define ADD_DATABASE_PARAMETER 0
#define PASS_DATABASE_TO_UI 0
#define CREATE_DATABASE_IN_APP 1

using std::vector;
using std::string;
using std::make_unique;
using std::cerr;


class UI {
  public:
#if !ADD_DATABASE_PARAMETER
    UI();
#else
    UI(FileDB &);
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
#if !CHANGE_DATABASE_TO_REFERENCE
    FileDB database;
#else
    FileDB &database;
#endif
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
#if PASS_DATABASE_TO_UI
    App()
    : ui(database)
    {
    }
#endif

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
#if CREATE_DATABASE_IN_APP
    FileDB database;
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



#if !ADD_DATABASE_PARAMETER
UI::UI()
#else
UI::UI(FileDB &database)
#endif
#if INITIALIZE_REFERENCE
: database(database)
#endif
{
  database.setStore(App::getStorageFile());
}



int main(int argc, char **argv)
{
  Launcher::main(vector<string>(argv+1, argv+argc));
}
