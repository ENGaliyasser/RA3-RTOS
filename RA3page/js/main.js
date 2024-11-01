(function ($) {
    "use strict";

    // Smooth scrolling on the navbar links
    $(".offcanvas a").on('click', function (event) {
        if (this.hash !== "") {
            event.preventDefault();
            
            $('html, body').animate({
                scrollTop: $(this.hash).offset().top - 45
            }, 1500, 'easeInOutExpo');
            
            if ($(this).parents('.offcanvas').length) {
                $('.offcanvas .active').removeClass('active');
                $(this).closest('a').addClass('active');
            }
        }
    });

    // Back to top button
    $(window).scroll(function () {
        if ($(this).scrollTop() > 100) {
            $('.back-to-top').fadeIn('slow');
        } else {
            $('.back-to-top').fadeOut('slow');
        }
    });
    $('.back-to-top').click(function () {
        $('html, body').animate({scrollTop: 0}, 1500, 'easeInOutExpo');
        return false;
    });

    // Code highlight
    $('pre code').each(function () {
        var that = $(this);
        var html = that.html().trim();
        that.empty();
        that.text(html);
    });
    hljs.highlightAll();
    
})(jQuery);

