#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <iostream>

App::App() {
    mapManager = std::make_shared<MapManager>(currentLevel);
    playerManager = std::make_shared<PlayerManager>(mapManager);
    enemyManager = std::make_shared<EnemyManager>(mapManager);
    playerManager->setCharaterManager(enemyManager);
    enemyManager->setCharaterManager(playerManager);

    selection = std::make_shared<Selection>();
    uiManager = std::make_shared<UIManager>(selection, mapManager, playerManager, enemyManager);
    camera = std::make_shared<Camera>(playerManager, enemyManager, mapManager, uiManager, selection);
    pc = std::make_shared<ProcessController>(mapManager, playerManager, enemyManager, selection, uiManager, camera);

    selection->setAbsolutePos(playerManager->getCharacter("Marth")->getAbsolutePos());
    camera->resetCameraAbsolutePos();
    uiManager->load();
    uiManager->update();
}

void App::Start() {
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;
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
        if (pc->ReturnCase()) {
            mapManager->loadMap(++currentLevel);
            playerManager->setInitialLevel(currentLevel);
            enemyManager->setInitialLevel(currentLevel);
            // playerManager->setCharaterManager(enemyManager);
            // enemyManager->setCharaterManager(playerManager);

            // selection = std::make_shared<Selection>();
            uiManager = std::make_shared<UIManager>(selection, mapManager, playerManager, enemyManager);      // rebuild
            camera = std::make_shared<Camera>(playerManager, enemyManager, mapManager, uiManager, selection); // rebuild
            pc = std::make_shared<ProcessController>(mapManager, playerManager, enemyManager, selection, uiManager,
                                                     camera);
            selection->setAbsolutePos(playerManager->getCharacter("Marth")->getAbsolutePos());
            camera->resetCameraAbsolutePos();
            uiManager->load();
            uiManager->update();
        }
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
    // player tip
    else if (Util::Input::IsKeyDown(Util::Keycode::F3) && accessInput) {
        LOG_INFO("F3 pressed");
        playerManager->changeTipsVisible(selection->getSelectCharacter());
        // next turn
    } else if (Util::Input::IsKeyDown(Util::Keycode::NUM_0) && selection->getStatus() == SelectionStatus::Normal) {
        for (auto &c : playerManager->getCurrentUnwaitedCharacters()) {
            playerManager->removeUnwaitingCharacter(c);
            c->setStatus(CharacterStatus::Waiting);
        }
        // cheating
    } else if (Util::Input::IsKeyDown(Util::Keycode::NUM_1))
        cheating = !cheating;

    // update
    camera->update();

    if (playerManager->getCharacter("Marth") == nullptr) {
        LOG_INFO("Marth is dead, game over!");
        uiManager->GameOver();
        return;
    }
    accessInput = uiManager->updateBattleUI();

    if (!--delayKeyCounter)
        delayKeyCounter = delayKeyLimit;

    // next turn
    if (playerManager->isNoMovableCharacter()) {
        if (cheating) {
            playerManager->reloadUnwaitingCharacter();
            enemyManager->reloadUnwaitingCharacter();
            selection->setStatus(SelectionStatus::Normal);
            return;
        }
        selection->setStatus(SelectionStatus::EnemyMoving);
        if (pc->enemyTurn(accessInput)) {
            playerManager->reloadUnwaitingCharacter();
            selection->setStatus(SelectionStatus::Normal);
        }
    }
    if (playerManager->update() && selection->getStatus() == SelectionStatus::Walking) {
        uiManager->loadActUI();
        selection->setStatus(SelectionStatus::SUI);
    }

    enemyManager->update();

    // std::cout << std::to_string(static_cast<int>(selection->getStatus())) << std::endl;
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
