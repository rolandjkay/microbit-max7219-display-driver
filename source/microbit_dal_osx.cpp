#ifdef TARGET_LIKE_OSX // Include only on OSX

#include <SDL2/SDL.h>

// Timer callback function which diverts event back to the main thread
/* with the same code as before: */
static Uint32 timer_callbackfunc(Uint32 interval, void *param)
{
  SDL_Event event;
  SDL_UserEvent userevent;

  /* In this example, our callback pushes a function
  into the queue, and causes our callback to be called again at the
  same interval: */

  userevent.type = SDL_USEREVENT;
  userevent.code = 0;

  event.type = SDL_USEREVENT;
  event.user = userevent;

  SDL_PushEvent(&event);
  return(interval);
}

void wait(float seconds)
{
  SDL_Event event;

  // Start the refresh timer
  Uint32 delay = seconds * 1000.;
  SDL_TimerID refresh_timer_id = SDL_AddTimer(delay, timer_callbackfunc, NULL);

  while (1)
  {
    SDL_WaitEvent(&event);
    switch (event.type)
    {
     case SDL_QUIT:
       exit(0);
       break;

     // Quit when the timer fires.
     case SDL_USEREVENT:
      SDL_RemoveTimer(refresh_timer_id);
      return;
    }
  }
}

#endif
