#include <memory>
#include <vector>


class Layout {
};


class GridBagLayout : public Layout {
};


class Widget {
  public:
    virtual ~Widget() {}
};


class Frame {
  public:
    void setSize(int /*width*/, int /*height*/)
    {
    }

    void setVisible(bool)
    {
    }

    void setLayout(std::unique_ptr<Layout>)
    {
    }

    void add(std::unique_ptr<Widget>)
    {
    }
};


class ComboBox : public Widget {
  public:
    ComboBox(const std::vector<std::string> &/*choices*/)
    {
    }
};
