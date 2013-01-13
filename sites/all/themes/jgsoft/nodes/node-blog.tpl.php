<div class="node<?php print $sticky ? ' sticky' : '' ?><?php print $status ? '' : ' node-unpublished' ?> node-<?php print $type ?> node-<?php print $type ?>-<?php print $page ? 'page' : 'teaser' ?>">
  <?php if (!$page): ?>
    <h2 class="title"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
    <div class="meta meta-header">
      <?php if (node_access('update', $node)): ?><a href="<?php print url('node/'. $node->nid .'/edit') ?>" class="editlink" title="<?php print t('Edit') ?>"> </a><?php endif; ?>
      <div class="submitted"><?php print $submitted ?></div>
      <?php if ($terms): ?><div class="terms"><?php print $terms ?></div><?php endif; ?>
    </div>
    <div class="content<?php print $field_images[0]['view'] ? ' content-has-images' : '' ?>">
      <?php if ($field_images[0]['view']): ?>
      <div class="left">
        <?php print $field_images[0]['view'] ?>
      </div>
      <?php endif ?>
      <div class="right">
        <?php print $node->content['body']['#value'] ?>
      </div>
      <div class="clear"></div>
    </div>
  <?php else: ?>
    <div class="meta meta-header">
      <?php if ($picture): print $picture; endif; ?>
      <?php if (node_access('update', $node)): ?><a href="<?php print url('node/'. $node->nid .'/edit') ?>" class="editlink" title="<?php print t('Edit') ?>"> </a>
      <div class="submitted"><?php print $submitted ?></div><?php endif; ?>
      <?php if ($terms): ?><div class="terms"><?php print $terms ?></div><?php endif; ?>
    </div>
    <div class="content">
      <?php if ($node->content['files']['#value']): ?>
        <?php print $node->content['files']['#value'] ?>
      <?php endif ?>

      <?php print $node->content['body']['#value'] ?>

      <ul class="images">
      <?php foreach($field_images as $image): ?>
        <li><?php print $image['view'] ?></li>
      <?php endforeach ?>
      </ul>

      <div class="meta meta-footer">
        <?php if ($links): ?><?php print $links ?><?php endif; ?>
      </div>
    </div>
  <?php endif; ?>
</div>
