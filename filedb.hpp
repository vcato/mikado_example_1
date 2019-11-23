class FileDB {
  public:
    void setStore(const std::string &)
    {
    }

    std::vector<std::string> load(const std::string &/*name*/)
    {
      return {"a", "b", "c"};
    }
};
