<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function background_block_admin_config($form, &$form_state) {
  
  $themes = list_themes();
  
  foreach ($themes as $theme_machine_name => $theme_info) {
      if ($theme_info->status != '1') unset($themes[$theme_machine_name]);
  }
  
  $themes_enabled_keys = array_keys($themes);
  
  $blocks = array();
  
  module_load_include('inc', 'block', 'block.admin');

  foreach ($themes_enabled_keys as $theme_enabled) {
      $blocks = array_merge($blocks, block_admin_display_prepare_blocks($theme_enabled));
  }
  
  $all_block_info = array();
  $allocated_block_info = array();
 
  foreach ($blocks as $block_id => $block) {
    $all_block_info[$block['bid']] = $block['theme'] . ' - ' . (strlen($block['title']) > 0 ? $block['title'] : $block['info']) . ' (' . $block['bid'] . ')';
  }
  
  $background_blocks = variable_get('background_blocks', array());
  
  if (count($background_blocks) > 0) {
      
    $rows = array();

    foreach ($background_blocks as $background_block_id) {
      $rows[] = array($background_block_id, $all_block_info[$background_block_id]);
      $allocated_block_info[$background_block_id] = $all_block_info[$background_block_id];
      unset($all_block_info[$background_block_id]);    
    }

    $form['allocated_background_blocks'] = array(
      '#title' => 'Background Blocks',
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
    );

    $form['allocated_background_blocks']['background_blocks'] = array(
      '#theme' => 'table',
      '#header' => array(
        'Block ID', 'title'
      ),
      '#rows' => $rows
    );

    $form['allocated_background_blocks']['blocks_allocated'] = array(
      '#title' => 'Blocks Allocated',
      '#type' => 'select',
      '#options' => $allocated_block_info,
    );
    $form['allocated_background_blocks']['block_remove'] = array(
      '#type' => 'submit',
      '#value' => 'Unset as background block',
    );
  }
  if (count($all_block_info) > 0) {
    $form['unallocated_background_blocks'] = array(
      '#title' => 'Unallocated Blocks',
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
    );

    $form['unallocated_background_blocks']['blocks_available'] = array(
      '#title' => 'Blocks Available',
      '#type' => 'select',
      '#options' => $all_block_info,
    );
    $form['unallocated_background_blocks']['block_add'] = array(
      '#type' => 'submit',
      '#value' => 'Set as background block',
    );
  }

  return $form;
}

function background_block_admin_config_submit($form, &$form_state) {

    switch($form_state['values']['op']) {
        case 'Set as background block' :
            $background_blocks = variable_get('background_blocks', array());
            $background_blocks[] = $form_state['values']['blocks_available'];
            variable_set('background_blocks', $background_blocks);
            break;
        case 'Unset as background block' :
            $background_blocks = variable_get('background_blocks', array());
            if(($key = array_search($form_state['values']['blocks_allocated'], $background_blocks)) !== false) {
                unset($background_blocks[$key]);
            }
            variable_set('background_blocks', $background_blocks);
            break;
    }
}