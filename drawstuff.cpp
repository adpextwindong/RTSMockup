DrawHandler c_clientDrawHandler;//Put all c_ITEMS with draw calls into the draw handler
    while (c_window.isOpen())
    {
        sf::Event event;
        while (c_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                c_window.close();
        }
		mouseLogic();//Ensure this can be put in client class
        c_window.clear();

		//printf("%s",c_selectionDrawState?"TRUE\n":"FALSE\n");
		c_window.draw(background);

		c_window.draw(tileSet,&tileSetTexture);
		c_window.draw(tileSet,tileSetStates);
		
		drawSelectionStroke(&c_playerSelection);
		drawUnitVector(&c_window,&s_playerUnits);
		drawUnitVector(&c_window,&s_enemyUnits);

		if(c_selectionDrawState==true){
			c_window.draw(c_clientSelectionShape);
		}

		theFPSCounter.updateFPSCounter();
		theFPSCounter.draw(&c_window);
        c_window.display();