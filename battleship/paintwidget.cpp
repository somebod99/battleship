#include "paintwidget.h"

#include <QMouseEvent>

#include <stack>

const QString h_labels[] = {
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J"
};

PaintWidget::PaintWidget(QWidget *parent)
    : QWidget(parent)
{
    field.resize(count);

    for(int i = 0; i < count; ++i)
    {
        field[i].resize(count);

        for(int j = 0; j < count; ++j)
        {
            field[i][j] = 0;
        }
    }
}

int PaintWidget::getCell(int i, int j)
{
    return field[i][j];
}

void PaintWidget::setCell(int i, int j, int val)
{
    field[i][j] = val;
    repaint();
}

void PaintWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setFont(QFont("Arial", 14));

    // draw h and v labels
    for(int i = 0; i < count; ++i)
    {
        painter.drawText(i * size + offset + size / 2 - 7, offset - 8, h_labels[i]);
        painter.drawText(offset - 25, i * size + offset + size / 2, QString::number(i + 1));
    }

    // draw grid
    for(int i = 0; i <= count; ++i)
    {
        painter.drawLine(offset, i * size + offset, count * size + offset, i * size + offset);
        painter.drawLine(i * size + offset, offset, i * size + offset, count * size + offset);
    }

    for(int i = 0; i < count; ++i)
    {
        for(int j = 0; j < count; ++j)
        {
            switch(field[i][j])
            {
            case 1:
                drawCell(painter, i, j);
                break;

            case 2:
                drawHit(painter, i, j);
                break;

            case 3:
                drawMiss(painter, i, j);
                break;

            }
        }
    }
}

void PaintWidget::mousePressEvent(QMouseEvent *)
{
    auto point = this->mapFromGlobal(QCursor::pos());

    if(point.x() < offset || point.y() < offset || point.x() > count * size + offset || point.y() > count * size + offset)
    {
        return;
    }

    int i = (point.y() - offset) / size;
    int j = (point.x() - offset) / size;

    if(field[i][j] != 2 && field[i][j] != 3)
    {
        emit press(i, j);
    }
}

void PaintWidget::drawCell(QPainter& p, int i, int j)
{
    p.setPen(Qt::black);
    p.setBrush(Qt::green);

    p.drawRect(offset + j * size, offset + i * size, size, size);
}

void PaintWidget::drawHit(QPainter &p, int i, int j)
{
    p.setPen(Qt::black);
    p.setBrush(Qt::red);

    p.drawRect(offset + j * size, offset + i * size, size, size);
    /*p.setPen(Qt::red);
    int x = offset + j * size;
    int y = offset + i * size;

    p.drawLine(x, y, x + size, y + size);
    p.drawLine(x + size, y, x, y + size);*/
}

void PaintWidget::drawMiss(QPainter &p, int i, int j)
{
    p.setPen(Qt::black);
    p.setBrush(Qt::black);

    p.drawEllipse(offset + j * size + size / 4, offset + i * size + size / 4, size / 2, size / 2);
}

void PaintWidget::checkLose()
{
    bool flag_lose = true;

    for(int i = 0; i < count && flag_lose; ++i)
    {
        for(int j = 0; j < count; ++j)
        {
            if(field[i][j] == 1)
            {
                flag_lose = false;
            }
        }
    }

    if(flag_lose)
    {
        emit lose();
    }
}

void PaintWidget::checkField()
{
    int four_deck = 0;
    int three_deck = 0;
    int two_deck = 0;
    int one_deck = 0;

    std::vector<std::vector<bool>> visited(count, std::vector<bool>(count));
    std::stack<std::pair<int,int>> s;

    int i = 0;
    int j = 0;

    while (i < field.size() && field[i][j] != 1)
    {
        ++j;

        if (j == field[i].size())
        {
            ++i;
            j = 0;
        }
    }

    if (i == field.size())
    {
        emit ready(false);
        return;
    }

    visited[i][j] = true;
    s.push({i, j});

    int count = 0;

    while (i < field.size())
    {
        if (s.empty())
        {
            switch(count)
            {
            case 4:
                four_deck++;
                break;

            case 3:
                ++three_deck;
                break;

            case 2:
                ++two_deck;
                break;

            case 1:
                ++one_deck;
                break;

            default:
                emit ready(false);
                return;
            }

            count = 0;

            while (i < field.size() && (field[i][j] != 1 || visited[i][j]))
            {
                ++j;

                if (j == field[i].size())
                {
                    ++i;
                    j = 0;
                }
            }

            if (i < field.size())
            {
                visited[i][j] = true;
                s.push({i, j});
            }
            else
            {
                break;
            }
        }

        auto [cur_i, cur_j] = s.top();
        s.pop();

        ++count;

        if (cur_j > 0 && field[cur_i][cur_j - 1] == 1 && !visited[cur_i][cur_j - 1])
        {
            visited[cur_i][cur_j - 1] = true;
            s.push({cur_i, cur_j - 1});
        }

        if (cur_j < field[cur_i].size() - 1 && field[cur_i][cur_j + 1] == 1 && !visited[cur_i][cur_j + 1])
        {
            visited[cur_i][cur_j + 1] = true;
            s.push({cur_i, cur_j + 1});
        }

        if (cur_i > 0 && field[cur_i - 1][cur_j] == 1 && !visited[cur_i - 1][cur_j])
        {
            visited[cur_i - 1][cur_j] = true;
            s.push({cur_i - 1, cur_j});
        }

        if (cur_i < field.size() - 1 && field[cur_i + 1][cur_j] == 1 && !visited[cur_i + 1][cur_j])
        {
            visited[cur_i + 1][cur_j] = true;
            s.push({cur_i + 1, cur_j});
        }
    }

    emit ready(four_deck == 1 && three_deck == 2 && two_deck == 3 && one_deck == 4);
}
