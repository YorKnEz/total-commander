#include "scrollbar.h"

Scrollbar createScrollbar(Font &font, int charSize, int x, int y, int width,
                          int height, int btnHeight, int scrollableHeight,
                          StateColors stateColors[B_MAX_STATES],
                          unsigned int borderThickness) {
  Scrollbar scrollbar;

  scrollbar.state = B_INACTIVE;
  scrollbar.scrollableHeight = scrollableHeight;
  scrollbar.scrollOffset = 0;

  scrollbar.up = createButton("/\\", font, charSize, x, y, width, btnHeight,
                              stateColors, borderThickness);
  scrollbar.down = createButton("\\/", font, charSize, x,
                                y + height - btnHeight + 2 * borderThickness,
                                width, btnHeight, stateColors, borderThickness);

  int trackHeight = height - 2 * btnHeight;

  scrollbar.track =
      createRect(x, y + btnHeight + 2 * borderThickness, width,
                 trackHeight - 2 * borderThickness,
                 stateColors[scrollbar.state].background,
                 stateColors[scrollbar.state].border, borderThickness);

  int displayHeight = trackHeight + btnHeight + borderThickness;
  int thumbHeight =
      (float(displayHeight) / float(scrollableHeight)) * float(trackHeight);

  if (thumbHeight > trackHeight) {
    thumbHeight = trackHeight;
  }

  scrollbar.thumb = createRect(x, y + btnHeight + 2 * borderThickness, width,
                               thumbHeight, stateColors[scrollbar.state].text,
                               stateColors[scrollbar.state].border, 0);

  return scrollbar;
}

float getScrollbarRatio(Scrollbar scrollbar) {
  return scrollbar.track.getGlobalBounds().height / scrollbar.scrollableHeight;
}

void updateScrollableHeight(Scrollbar &scrollbar, int scrollableHeight) {
  FloatRect bounds = scrollbar.thumb.getGlobalBounds();

  int trackHeight = scrollbar.track.getGlobalBounds().height;
  int displayHeight =
      trackHeight + scrollbar.down.background.getGlobalBounds().height;
  int thumbHeight =
      (float(displayHeight) / float(scrollableHeight)) * float(trackHeight);

  if (thumbHeight > trackHeight) {
    thumbHeight = trackHeight;
  }

  scrollbar.thumb.setSize(Vector2f(bounds.width, thumbHeight));

  scrollbar.scrollableHeight = scrollableHeight;
}

void updateScrollbar(Scrollbar &scrollbar, int scrollOffset) {
  Vector2f position = scrollbar.track.getPosition();

  scrollbar.scrollOffset = -getScrollbarRatio(scrollbar) * float(scrollOffset);
  scrollbar.thumb.setPosition(
      Vector2f(position.x, position.y + scrollbar.scrollOffset));
}

void drawScrollbar(RenderWindow &window, Scrollbar scrollbar) {
  drawButton(window, scrollbar.up);
  drawButton(window, scrollbar.down);

  window.draw(scrollbar.track);
  window.draw(scrollbar.thumb);
}
