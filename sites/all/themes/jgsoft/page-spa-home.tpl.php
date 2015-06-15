<!DOCTYPE html>
<html lang="en" dir="ltr">
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta charset="UTF-8" />
  <?php print $head ?>
  <title><?php print $head_title ?></title>
  <style type="text/css" media="all">
    @import url("https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css");
    @import url("https://maxcdn.bootstrapcdn.com/bootswatch/3.3.4/flatly/bootstrap.min.css");
    @import url("https://maxcdn.bootstrapcdn.com/font-awesome/4.3.0/css/font-awesome.min.css");
    @import url("<?php print base_path().path_to_theme() ?>/css/spa/main.css");
  </style>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/js/bootstrap.min.js"></script>
  <script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.3.15/angular.min.js"></script>
  <script type="text/javascript"><?php /* Needed to avoid Flash of Unstyle Content in IE */ ?> </script>
</head>
<body>
  <div id="main-nav" class="navbar navbar-default navbar-fixed-top">
    <div class="row">
      <div class="col-lg-12">
        <div class="gutter">
          <div class="col-lg-1">
          </div>
          <div class="col-lg-1 spa-container">
            <div id="selfie"><img src="/sites/default/files/images/green_instgrm.JPG" /></div>
          </div>
          <div class="col-lg-4">
            <?php if ($site_name): ?>
              <h1 class="site-title"><a href="<?php print $front_page ?>" title="<?php print $site_name ?>"><?php print $site_name ?></a></h1>
            <?php endif; ?>
          </div>
          <div class="col-lg-2">
              <p class="contact-icons">
                <a href="https://facebook.com/eodoxus" target="_blank" class="btn btn-default btn-sm"><i class="fa fa-facebook"></i></a>
                <a href="https://www.linkedin.com/in/jasongordon" target="_blank" class="btn btn-default btn-sm"><i class="fa fa-linkedin"></i></a>
                <a href="https://github.com/eodoxus" target="_blank" class="btn btn-default btn-sm"><i class="fa fa-github"></i></a>
                <a href="mailto:{{user.email}}" class="btn btn-default btn-sm"><i class="fa fa-envelope"></i></a>
              </p>
          </div>
          <div class="col-lg-3 spa-container">
            <p class="contact-info">
              {{user.email}} <br />
              {{user.phone}}
            </p>
          </div>
          <div class="col-lg-1">
          </div>
        </div>
      </div>
    </div>
  </div>
  <div id="content">
      <div class="row">
        <div class="col-lg-12">
          {{projects}}
        </div>
      </div>
      <div class="row row-dark">
        <div class="col-lg-12">
          {{photos}}
        </div>
      </div>
      <div class="row">
        <div class="col-lg-12">
          {{bio}}
        </div>
      </div>
  </div>
</body>
</html>