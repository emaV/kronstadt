<?php 
// $Id: node.tpl.php,v 1.1 2009/02/28 23:33:58 jwolf Exp $
$submitted = NULL;
$content = $node->content['thumbnail']['#value'];
$node_bottom = $node->content['body']['#value'];

?>

<!-- start node.tpl.php -->
<div id="node-<?php print $node->nid; ?>" class="node <?php print $node_classes; ?>">
  <?php print $picture ?>

  <?php if ($page == 0): ?>
  <h3 class="title"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h3>
  <?php endif; ?>

  <div class="meta">
    <?php if ($submitted): ?>
    <span class="submitted"><?php print $submitted ?></span>
    <?php endif; ?>
  </div>
  
  <?php if ($terms): ?>
  <div class="terms">
    <?php print $terms; ?>
  </div>
  <?php endif;?>
  
  <div class="content">
    <?php print $content ?>
  </div>
  
  <?php if ($links): ?>
  <div class="links">
    <div class="corner top-right"></div>
    <div class="corner top-left"></div>
    <div class="inner">
      <?php print $links; ?>
    </div>
    <div class="corner bottom-right"></div>
    <div class="corner bottom-left"></div>
  </div>
  <?php endif; ?>

  <?php if ($node_bottom || !$teaser): ?>
  <div id="node-bottom">
    <?php print $node_bottom; ?>
  </div>
  <?php endif; ?>
</div>
<!-- /#node-<?php print $node->nid; ?> -->
