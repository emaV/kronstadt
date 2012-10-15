<?php
// $Id: node.tpl.php,v 1.1 2009/11/01 01:47:10 himerus Exp $
?>
<?php if (!$page): ?>
<div class="title-wrapper">
<h2 class="node-title"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<span class="comment-bubble"><?php print $comment_bubble; ?></span>
<span class="comment-bubble-add"><?php print $comment_bubble_add; ?></span>
</div>
<?php endif; ?>
<div<?php print $attributes; ?>>
  
  
  <div class="node-info alpha grid-<?php print $node_info_width; ?>">
	  <?php print $picture ?>
	  <div class="submitted"><?php print $submitted ?></div>
	  <?php if($terms): ?>
	    <div class="taxonomy"><?php print $terms ?></div>
	  <?php endif; ?>
	  <?php if($service_links): ?>
	    <?php print $service_links; ?>
	  <?php endif; ?>
	  
  </div>
  <div class="node-body omega grid-<?php print $node_body_width; ?>">
	  <?php print $content ?>
    <div class="node-links"><?php print $links; ?></div>
  </div>
</div>