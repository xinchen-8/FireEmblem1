#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <iostream>

App::App() {
    playerManager->setCharaterManager(enemyManager);
    enemyManager->setCharaterManager(playerManager);
    selection->setAbsolutePos(playerManager->getCharacter("Marth")->getAbsolutePos());
    camera->resetCameraAbsolutePos();

    // playerManager->refreshAllCharacterMoveRange();
    // LOG_INFO("Player Move Manager Refresh Success.");
    // enemyManager->refreshAllCharacterMoveRange();
    // LOG_INFO("Enemy Move Manager Refresh Success.");
}

void App::Start() {
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;
    uiManager->load();
    uiManager->update();
}

void App::Update() {
    // end
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    // selection move
    if (((Util::Input::IsKeyPressed(Util::Keycode::UP) && delayKeyCounter <= delayKeyCheck) ||
         (Util::Input::IsKeyDown(Util::Keycode::UP))) &&
        selection->moveJudge(Forword::Up, mapManager->getMapSize()) && accessInput) {

        pc->MovCase({0, TILE_SIZE});
        delayKeyCounter = delayKeyLimit;
    }

    else if (((Util::Input::IsKeyPressed(Util::Keycode::DOWN) && delayKeyCounter <= delayKeyCheck) ||
              (Util::Input::IsKeyDown(Util::Keycode::DOWN))) &&
             selection->moveJudge(Forword::Down, mapManager->getMapSize()) && accessInput) {

        pc->MovCase({0, -TILE_SIZE});
        delayKeyCounter = delayKeyLimit;
    }

    else if (((Util::Input::IsKeyPressed(Util::Keycode::LEFT) && delayKeyCounter <= delayKeyCheck) ||
              (Util::Input::IsKeyDown(Util::Keycode::LEFT))) &&
             selection->moveJudge(Forword::Left, mapManager->getMapSize()) && accessInput) {

        pc->MovCase({-TILE_SIZE, 0});
        delayKeyCounter = delayKeyLimit;
    }

    else if (((Util::Input::IsKeyPressed(Util::Keycode::RIGHT) && delayKeyCounter <= delayKeyCheck) ||
              (Util::Input::IsKeyDown(Util::Keycode::RIGHT))) &&
             selection->moveJudge(Forword::Right, mapManager->getMapSize()) && accessInput) {

        pc->MovCase({TILE_SIZE, 0});
        delayKeyCounter = delayKeyLimit;
    }
    // selectedAct UI move
    if (((Util::Input::IsKeyPressed(Util::Keycode::UP) && delayKeyCounter <= delayKeyCheck) ||
         (Util::Input::IsKeyDown(Util::Keycode::UP))) &&
        accessInput) {

        pc->MovCase(-1);
        delayKeyCounter = delayKeyLimit;
    }

    else if (((Util::Input::IsKeyPressed(Util::Keycode::DOWN) && delayKeyCounter <= delayKeyCheck) ||
              (Util::Input::IsKeyDown(Util::Keycode::DOWN))) &&
             accessInput) {

        pc->MovCase(1);
        delayKeyCounter = delayKeyLimit;
    }
    // select character method
    if (Util::Input::IsKeyDown(Util::Keycode::RETURN) && accessInput) {
        LOG_INFO("Enter pressed");
        pc->ReturnCase();
    }

    // go back
    else if (Util::Input::IsKeyDown(Util::Keycode::BACKSPACE) && accessInput) {
        LOG_INFO("Backspace pressed");

        // SelectionStatus status = selection->getStatus();
        // std::shared_ptr<Character> selectedCharacter = selection->getSelectCharacter();

        // //select attacked target => back to UI
        // if(status == SelectionStatus::Targeting){
        //     selection->setStatus(SelectionStatus::SUI);
        //     selection->setAbsolutePos(selectedCharacter->getAbsolutePos());
        //     selectedCharacter->resetRange();
        //     playerManager->buildCharacterTips(selectedCharacter);
        // }
        // //select walk (UI) => back to select character
        // if(status == SelectionStatus::SUI){
        //     if(!selectedCharacter) LOG_DEBUG("No character selectedAct");

        //     selectedCharacter->setAbsolutePos(selection->getOriginalSelectionPos());
        //     selectedCharacter->setForword(Forword::Down);
        //     selectedCharacter->setStatus(CharacterStatus::Moving);
        //     selectedCharacter->clearWalkPath();

        //     selection->setMoveLimit(playerManager->selectCharacter(selectedCharacter));
        //     selection->setStatus(SelectionStatus::Moving);
        //     selection->setAbsolutePos(selection->getOriginalSelectionPos());
        //     camera->resetCameraAbsolutePos();
        // }
        // //select character => back to none
        // else if(status == SelectionStatus::Moving){
        //     if(!selectedCharacter) LOG_DEBUG("No character selectedAct");

        //     selectedCharacter->setAbsolutePos(selection->getOriginalSelectionPos());
        //     selectedCharacter->setStatus(CharacterStatus::Normal);
        //     selection->setStatus(SelectionStatus::Normal);
        //     selection->setAbsolutePos(selection->getOriginalSelectionPos());
        //     selection->setSelectCharacter(nullptr);
        //     playerManager->clearTips();
        //     camera->resetCameraAbsolutePos();
        // }
    }

    // info UI
    if (Util::Input::IsKeyDown(Util::Keycode::F1) && accessInput) {
        LOG_INFO("F1 pressed");

        uiManager->changeVisibleTileInfo();
    }
    // character info UI
    else if (Util::Input::IsKeyDown(Util::Keycode::F2) && accessInput) {
        LOG_INFO("F2 pressed");
        uiManager->changeVisibleCharacterInfoFull();
    }
    // item info UI
    else if (Util::Input::IsKeyDown(Util::Keycode::F3) && accessInput) {
        LOG_INFO("F3 pressed");
        uiManager->changeVisibleItemInfo();
    }
    // 物品切換控制
    else if (Util::Input::IsKeyDown(Util::Keycode::Q) && accessInput && uiManager->getItemInfo()->getVisible()) {
        LOG_INFO("Q pressed - Previous item");
        uiManager->prevItemInfo();
    }
    else if (Util::Input::IsKeyDown(Util::Keycode::E) && accessInput && uiManager->getItemInfo()->getVisible()) {
        LOG_INFO("E pressed - Next item");
        uiManager->nextItemInfo();
    }
    // player tip
    else if (Util::Input::IsKeyDown(Util::Keycode::F4) && accessInput) {
        LOG_INFO("F4 pressed");
        playerManager->changeTipsVisible(selection->getSelectCharacter());
    } else if (Util::Input::IsKeyDown(Util::Keycode::NUM_0)) {
        playerManager->reloadUnwaitingCharacter();
    }

    // update
    camera->update();

    if (playerManager->update() && selection->getStatus() == SelectionStatus::Walking) {
        uiManager->loadActUI();
        selection->setStatus(SelectionStatus::SUI);
    }
    enemyManager->update();
    accessInput = uiManager->updateBattleUI();

    if (!--delayKeyCounter)
        delayKeyCounter = delayKeyLimit;

    // next turn
    if (playerManager->isNoMovableCharacter())
        playerManager->reloadUnwaitingCharacter();
    // std::cout << std::to_string(static_cast<int>(selection->getStatus())) << std::endl;
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
