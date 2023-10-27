#ifndef VIEW_HPP_
#define VIEW_HPP_

#include <vector>
#include <string>
#include <functional>
#include "Save.hpp"

constexpr size_t const WIDTH{60};

void renderTitle();
void renderHelper(int default_checks_);
void renderSelectData(std::vector<std::string> const& datas_);
void renderEnding(std::string warning_);
void renderMenu(const std::function<void()>& draw_function_, bool save_, std::string warning_);
void renderNewGame(bool save_);
void renderLoadSave(std::vector<std::pair<std::string, Save>> const& saves_);
void renderRequestSave();
void renderOverwriteConfirm();
void renderAbout();
void renderQuitting(bool save_);

#endif /// VIEW_HPP_