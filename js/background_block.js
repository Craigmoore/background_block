
// Using the closure to map jQuery to $.
(function ($) {
    
    // Store our function as a property of Drupal.behaviors.
    Drupal.behaviors.background_block = {
        
      attach: function (context, settings) {

          var $body = $('body');
          
          $.each(settings.background_block, function(i, v) {
              var $background_block = $('.'+v);
              $background_block.once('background_block', function(){
                var $block_container = $background_block.parent().parent().detach();
                $body.append($block_container);
                $block_container.css({
                  'position' : 'absolute',
                  'top' : '0px',
                  'bottom' : '0px',
                  'width' : '100%',
                  'height' : $body.height(),
                  'z-index' : -1000
                });
              });
          });
      }
    };

        
}(jQuery));

