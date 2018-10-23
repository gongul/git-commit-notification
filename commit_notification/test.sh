#!/bin/dash

cat <(crontab -l) <(echo "0 0 * * * /tmp/notification-test/daily-notification.sh") | crontab -



