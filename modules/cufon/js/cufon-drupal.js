(function ($) {
  Drupal.behaviors.cufonReplace = function(context) {
    for (o in Drupal.settings.cufonSelectors) {
      var s = Drupal.settings.cufonSelectors[o];
      $(s.selector + ':not(.cufon-replace-processed)', context).addClass('cufon-replace-processed').each(function() {
        Cufon.replace($(this), s.options);
      });
    }

    // Work around Internet Explorer rendering delay
    Cufon.now();
  };
})(jQuery);