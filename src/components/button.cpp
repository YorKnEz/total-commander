#include "button.h"

Button createButton(string text, Font &font, int charSize, int x, int y,
                    int width, int height,
                    StateColors stateColors[B_MAX_STATES],
                    unsigned int borderThickness, Texture *icon) {
  Button button;
  button.state = B_INACTIVE; // set the state of the button
  button.fullText =
      text; // save the string that is supposed to be shown on the screen

  // copy the state colors
  for (int i = 0; i < B_MAX_STATES; i++) {
    button.stateColors[i] = stateColors[i];
  }

  // initialize the background of the button
  button.background =
      createRect(x, y, width, height, stateColors[button.state].background,
                 stateColors[button.state].border, borderThickness);

  FloatRect backgroundBounds = button.background.getGlobalBounds();

  // initialize the text of the button
  button.text = createText(text, font, charSize, x, y,
                           backgroundBounds.width - 2 * (borderThickness + 1),
                           stateColors[button.state].text);

  FloatRect textBounds = button.text.getLocalBounds();

  // set the offset to the text relative to the button
  int offsetX = backgroundBounds.width / 2 - 1;

  if (text.size() > 0) {
    offsetX -= (textBounds.width / 2 + textBounds.left);
  }

  int offsetYText = (backgroundBounds.height - charSize) / 2;

  if (icon) {
    button.icon.setTexture(*icon);

    FloatRect iconBounds = button.icon.getGlobalBounds();

    offsetX -= iconBounds.width / 2;

    if (text.size() > 0) {
      offsetX -= 2; // add some space between the icon and text
    }

    int offsetYIcon = (backgroundBounds.height - iconBounds.height) / 2;

    button.text.setPosition(x + iconBounds.width + 4 + offsetX,
                            y + offsetYText);
    button.icon.setPosition(x + offsetX, y + offsetYIcon);
  } else {
    button.text.setPosition(x + offsetX, y + offsetYText);
  }

  return button;
}

void updateButtonState(Button &button, Event event, MouseEventType type,
                       Vector2i &oldClick) {
  FloatRect buttonBounds =
      button.background.getGlobalBounds(); // get bounds of button

  switch (type) {
  case RELEASE:
    if (button.state == B_CLICKED || button.state == B_DCLICKED) {
      button.state =
          isHovered(buttonBounds, event.mouseButton.x, event.mouseButton.y)
              ? B_HOVERED
              : B_INACTIVE;
    }
    break;
  case DCLICK:
    // if a double click happens, then we execute the if, else we jump to the
    // simple click case, which is guarenteed to handle the event
    if (isHovered(buttonBounds, event.mouseButton.x, event.mouseButton.y) &&
        isHovered(buttonBounds, oldClick.x, oldClick.y)) {
      button.state = B_DCLICKED;
      oldClick = Vector2i(event.mouseButton.x, event.mouseButton.y);
      break;
    }
  case CLICK:
    button.state =
        isHovered(buttonBounds, event.mouseButton.x, event.mouseButton.y)
            ? B_CLICKED
            : B_INACTIVE;

    if (button.state == B_CLICKED) {
      oldClick = Vector2i(event.mouseButton.x, event.mouseButton.y);
    }
    break;
  case MOVE:
    if (button.state != B_CLICKED && button.state != B_DCLICKED) {
      button.state =
          isHovered(buttonBounds, event.mouseMove.x, event.mouseMove.y)
              ? B_HOVERED
              : B_INACTIVE;
    }
    break;
  }
}

void updateButtonIcon(Button &button, Texture *newIcon) {
  button.icon.setTexture(*newIcon);

  FloatRect backgroundBounds = button.background.getGlobalBounds();
  FloatRect iconBounds = button.icon.getGlobalBounds();
  FloatRect textBounds = button.text.getLocalBounds();

  // set the offset to the text relative to the button
  int offsetX =
      (backgroundBounds.width - textBounds.width) / 2 - textBounds.left;

  int offsetYText =
      (backgroundBounds.height - button.text.getCharacterSize()) / 2;
  int offsetYIcon = backgroundBounds.height / 2 - iconBounds.height / 2;

  if (newIcon) {
    offsetX -= iconBounds.width / 2;

    if (button.fullText.size() > 0) {
      offsetX -= 4; // add some space between the icon and text
    }

    button.text.setPosition(backgroundBounds.left + iconBounds.width + 8 +
                                offsetX,
                            backgroundBounds.top + offsetYText);
  } else {

    button.text.setPosition(backgroundBounds.left + offsetX,
                            backgroundBounds.top + offsetYText);
  }

  button.icon.setPosition(backgroundBounds.left + offsetX,
                          backgroundBounds.top + offsetYIcon);
}

void updateButtonTheme(Button &button, StateColors stateColors[B_MAX_STATES]) {
  // copy the state colors
  for (int i = 0; i < B_MAX_STATES; i++) {
    button.stateColors[i] = stateColors[i];
  }
  button.background.setFillColor(stateColors[button.state].background);
  button.background.setOutlineColor(stateColors[button.state].border);
  button.text.setFillColor(stateColors[button.state].text);
}

void drawButton(RenderWindow &window, Button button) {
  // update the color of the button depending on it's state
  button.text.setFillColor(button.stateColors[button.state].text);
  button.background.setFillColor(button.stateColors[button.state].background);
  button.background.setOutlineColor(button.stateColors[button.state].border);

  // draw the button on the window passed by reference
  window.draw(button.background);
  window.draw(button.text);

  if (button.icon.getTexture()) {
    window.draw(button.icon);
  }
}
