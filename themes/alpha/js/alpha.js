$(document).ready(function(){
	/**
	 * Custom effects for the Service Links Module
	 */
	$('.service-links a').fadeTo(10, 0.35);
	$('.service-links a').hover(function(){
		$(this).fadeTo(50, 1);
	}, function(){
		$(this).fadeTo(50, 0.35);
	});
	/**
	 * Custom Manipulation of the user pictures.
	 */
	$('.picture').fadeTo(150, 0.75);
	$('.picture').hover(function(){
		$(this).fadeTo(150, 1);
	}, function(){
		$(this).fadeTo(250, 0.75);
	});
	
	
	
});