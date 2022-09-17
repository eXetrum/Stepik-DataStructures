// 2022.09.06 v1
// Promotion scripts
document.addEventListener('DOMContentLoaded', function() {
  var today = new Date();

  /* if search parameter "testPromo" is present, show promotion */
  var testPromo = '';
  if (window.location.search.indexOf('testPromo') > -1) {
    testPromo = new URL(window.location.href).searchParams.get('testPromo').toUpperCase();
  }

  //September promotion date range from September 12 7:00am PT to September 27 11:59pm PT
  sepStartDate = new Date('2022-09-12T07:00:00.000-07:00');
  sepExtraDate = new Date('2022-09-22T23:59:00.000-07:00');
  sepEndDate = new Date('2022-09-27T23:59:00.000-07:00');

  //if during date range,
  //replace exit intent overlay with September promotion
  if ((today > sepStartDate && today <= sepEndDate) || testPromo === 'SEPTEMBER') {
    // Target exit intent overlay
    const overlay = document.querySelector('#exitIntentOverlay');
    if (overlay) {
      overlay.classList.add('exitIntentOverlay-js');

      // Load css file for promotion
      var cssFileRef = document.createElement("link");
      cssFileRef.rel = "stylesheet";
      cssFileRef.type = "text/css";
      cssFileRef.href = "https://cdn.oreillystatic.com/assets/css/anybird/exit-intent-overlay-promo.css";
      document.getElementsByTagName("head")[0].appendChild(cssFileRef);

      //Replace overlay text
      const overlayText = overlay.querySelector('.exitIntentOverlay-text');
      if (today < sepExtraDate) {
        overlayText.innerHTML = '<h2>Lucky you.</h2><p>You can get this and 59,000+ other books and videos. Live online courses and events. Labs and sandboxes. Certification prep and practice exams. And more—all for an exclusive sale price. <strong>But hurry, offer <span class="nowrap">expires 9/22.</span></strong></p><a class="exitIntentOverlay-cta" href="https://www.oreilly.com/online-learning/sep2022-learn-more-for-less.html?code=SEPTEMBERSAVE22&utm_medium=exit+popup&utm_source=anybird&utm_campaign=sept+sale+2022&utm_content=see+the+deal">See the deal</a>';
      }
      else {
        overlayText.innerHTML = '<h2>Lucky you.</h2><p>You can get this and 59,000+ other books and videos. Live online courses and events. Labs and sandboxes. Certification prep and practice exams. And more—all for an exclusive sale price. <strong>But hurry, offer <span class="nowrap">expires 9/27.</span></strong></p><a class="exitIntentOverlay-cta" href="https://www.oreilly.com/online-learning/sep2022-learn-more-for-less.html?code=SEPTEMBERSAVE22&utm_medium=exit+popup&utm_source=anybird&utm_campaign=sept+sale+2022&utm_content=see+the+deal">See the deal</a>';
      }
      //Replace overlay figure
      const overlayFigure = overlay.querySelector('.exitIntentOverlay-figure');
      overlayFigure.innerHTML = '<img src="https://cdn.oreillystatic.com/assets/images/anybird/anybird-sep2022-animation.gif" alt="">';
    }
  }
});